#include "main.h"

void main_cleanup() {
  printf("Cleaning up.\n");
  close(connection_socket);
  close(client_socket);
  exit(0);
}

void main_init_clients() {
  int i;

  for (i = 0; i < config->n_threads; i++) {
    pthread_mutex_init(&thread_locks[i], NULL);
    pthread_mutex_init(&cond_lock[i], NULL);
    pthread_cond_init(&wait_for_work[i], NULL);
    thread_ready[i] = 1;

    workers[i].id = i;
    workers[i].ready = &thread_ready[i];
    workers[i].lock = &thread_locks[i];
    workers[i].sem_threads = sem_threads;
    workers[i].request = &requests[i];
    workers[i].wait_for_work = &(wait_for_work[i]);
    workers[i].cond_lock = &(cond_lock[i]);
  }
}

void main_init_semaphores() {
  sem_unlink("buffer_full");
  sem_buffer_full = sem_open("buffer_full", O_CREAT|O_EXCL, 0700, 20);

  sem_unlink("buffer_empty");
  sem_buffer_empty = sem_open("buffer_empty", O_CREAT | O_EXCL, 0700, 0);

  sem_unlink("threads");
  sem_threads = sem_open("threads", O_CREAT | O_EXCL, 0700, 10);
}

void main_init_scheduler() {
  thread_ready = (int*) malloc(sizeof(int) * config->n_threads);
  requests = (http_request**) malloc(sizeof(http_request*) * config->n_threads);
  thread_locks = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t) * config->n_threads);
  wait_for_work = (pthread_cond_t*) malloc(sizeof(pthread_cond_t) * config->n_threads);
  cond_lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t) * config->n_threads);
  workers = (client_data*) malloc(sizeof(client_data) * config->n_threads);
  client_threads = (pthread_t*) malloc(sizeof(pthread_t) * config->n_threads);
  
  scheduler_args = (scheduler_data*) malloc(sizeof(scheduler_data));
  scheduler_args->buffer = request_buffer;
  scheduler_args->sem_buffer_empty = sem_buffer_empty;
  scheduler_args->sem_buffer_full = sem_buffer_full;
  scheduler_args->sem_threads = sem_threads;
  scheduler_args->policy = FIFO_POLICY;
  scheduler_args->thread_ready = thread_ready;
  scheduler_args->thread_locks = thread_locks;
  scheduler_args->wait_for_work = wait_for_work;
  scheduler_args->requests = requests;
}

void main_init_config() {
  pid_t config_process = fork();

  if (config_process == -1) {
    fprintf(stderr, "Fork error.\n");
  } else if (config_process == 0) {
    config_start(config);
    exit(0);
  } else {
    wait(NULL);
  }
}

void main_init_shared_memory() {
  configuration_shmid = shmget(IPC_PRIVATE, sizeof(config_t), IPC_CREAT | 0700);
  config = (config_t*) shmat(configuration_shmid, NULL, 0);
}

void main_init() {
  int i;

  main_init_shared_memory();
  main_init_config();

  printf("Number of threads in config: %d\n", config->n_threads);
  
  request_buffer = buffer_create(config->n_threads * 2);

  main_init_semaphores();

  connection_socket = connection_start(3000); // FIXME config->port
  client_name_len = sizeof(client_name);

  main_init_scheduler();
  main_init_clients();

  pthread_create(&scheduler_thread, NULL, scheduler_code, scheduler_args);

  for (i = 0; i < config->n_threads; i++) {
    pthread_create(&client_threads[i], NULL, client_code, &(workers[i]));
  }

  signal(SIGINT, main_cleanup);
}

int main(void) {
  main_init();

  while (true) {
    if ((client_socket = accept(connection_socket,
                                (struct sockaddr *) &client_name,
                                &client_name_len)) == -1 ) {
      printf("Error accepting connection.\n");
      exit(1);
    }

    http_request *request = (http_request*) malloc(sizeof(http_request));
    http_parse_request(client_socket, request);

    sem_wait(sem_buffer_full);
    buffer_add(request_buffer, request);
    sem_post(sem_buffer_empty);
  }

  return 0;
}

#include "main.h"

void main_shutdown_threads() {
  printf("%d\n", config->n_threads);

  pthread_kill(scheduler_thread, SIGUSR2);
  pthread_join(scheduler_thread, NULL);

  int i;
  for (i = 0; i < config->n_threads; i++) {
    pthread_kill(client_threads[i], SIGUSR1);
    pthread_join(client_threads[i], NULL);
  }

  printf("Threads Killed\n");
}

void main_shutdown_processes() {
  // TODO
}

void main_free_thread_memory() {
  free(thread_ready);
  free(requests);
  free(thread_locks);
  free(wait_for_work);
  free(cond_lock);
  free(workers);
  free(client_threads);
  free(scheduler_args);

  printf("Memory Released\n");
}

void main_shutdown() {
  main_shutdown_threads();
  main_free_thread_memory();
  main_shutdown_processes();

  close(connection_socket);
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
  sem_buffer_full = sem_open("buffer_full", O_CREAT | O_EXCL, 0700, config->n_threads * 2);

  sem_unlink("buffer_empty");
  sem_buffer_empty = sem_open("buffer_empty", O_CREAT | O_EXCL, 0700, 0);

  buffer_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(buffer_mutex, NULL);

  sem_unlink("threads");
  sem_threads = sem_open("threads", O_CREAT | O_EXCL, 0700, config->n_threads);
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
  scheduler_args->n_threads = config->n_threads;
  scheduler_args->buffer = request_buffer;
  scheduler_args->sem_buffer_empty = sem_buffer_empty;
  scheduler_args->sem_buffer_full = sem_buffer_full;
  scheduler_args->buffer_mutex = buffer_mutex;
  scheduler_args->sem_threads = sem_threads;
  scheduler_args->policy = FIFO_POLICY;
  scheduler_args->thread_ready = thread_ready;
  scheduler_args->thread_locks = thread_locks;
  scheduler_args->wait_for_work = wait_for_work;
  scheduler_args->requests = requests;
}

void main_init_stats() {
  stats_process = fork();

  if (stats_process == -1) {
    fprintf(stderr, "Fork error creating stats process.\n");
  } else if (stats_process == 0) {
    stats_start(message_queue_id, "logs.txt");
    exit(0);
  } else { }
}

void main_init_message_queue() {
  message_queue_id = msgget(IPC_PRIVATE, IPC_CREAT | 0777);

  if (message_queue_id < 0) {
    fprintf(stderr, "An error occurred creating the message queue.\n");
    exit(1);
  }
}

void main_init_config() {
  pid_t config_process = fork();

  if (config_process == -1) {
    fprintf(stderr, "Fork error creating config process.\n");
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

  main_init_message_queue();
  main_init_stats();

  main_init_shared_memory();
  main_init_config();

  printf("Number of threads in config: %d\n", config->n_threads);

  request_buffer = buffer_create(config->n_threads * 2);

  main_init_semaphores();

  connection_socket = connection_start(config->port);
  client_name_len = sizeof(client_name);

  main_init_scheduler();
  main_init_clients();

  pthread_create(&scheduler_thread, NULL, scheduler_code, scheduler_args);

  for (i = 0; i < config->n_threads; i++) {
    pthread_create(&client_threads[i], NULL, client_code, &(workers[i]));
  }

  signal(SIGINT, main_shutdown);
}

void sigtstp_handle() {
  close(connection_socket);
  close(client_socket);
  msgctl(message_queue_id, IPC_RMID, 0);

  kill(stats_process, SIGINT);

  main_init();
}

int main(void) {
  main_init();

  signal(SIGTSTP, sigtstp_handle);
  
  while (true) {
    if ((client_socket = accept(connection_socket,
                                (struct sockaddr *) &client_name,
                                &client_name_len)) == -1) {
      fprintf(stderr, "Error accepting connection.\n");
      //return 1;
    }

    http_request *request = (http_request*) malloc(sizeof(http_request));
    http_parse_request(client_socket, request);
    request->message_queue_id = message_queue_id;

    sem_wait(sem_buffer_full);
    pthread_mutex_lock(buffer_mutex);
    buffer_add(request_buffer, request);
    pthread_mutex_unlock(buffer_mutex);
    sem_post(sem_buffer_empty);
  }

  return 0;
}

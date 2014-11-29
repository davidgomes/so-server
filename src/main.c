
#include "main.h"

void cleanup() {
  printf("Cleaning up.\n");
  close(connection_socket);
  close(client_socket);
  exit(0);
}

void init_clients() {
  int i;

  //pthread_mutex_lock(&myMutex);
  //pthread_mutex_unlock(&myMutex);

  for(i=0; i<10; i++) {
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
  //sleep(1);
}

void init_semaphores() {
  sem_unlink("buffer_full");
  sem_buffer_full = sem_open("buffer_full",O_CREAT|O_EXCL,0700, 20);
  sem_unlink("buffer_empty");
  sem_buffer_empty = sem_open("buffer_empty",O_CREAT|O_EXCL,0700, 0);
  sem_unlink("threads");
  sem_threads = sem_open("threads",O_CREAT|O_EXCL,0700, 10);
}

void init_scheduler() {
  scheduler_args = (scheduler_data*)malloc(sizeof(scheduler_data));
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

void init() {
  int i;

  request_buffer = buffer_create(20); // replace 20 with 2 * NUM_THREADS
  
  init_semaphores();

  connection_socket = connection_start(3000);
  client_name_len = sizeof(client_name);
  
  init_scheduler();
  init_clients();

  pthread_create(&scheduler_thread, NULL, scheduler_code, scheduler_args);

  for(i=0; i<10; i++) {
    pthread_create(&client_threads[i], NULL, client_code, &(workers[i]));
  }

  signal(SIGINT, cleanup);
}

int main(void) {
  init();

  pid_t config_process = fork();

  if (config_process == -1) {
    fprintf(stderr, "Fork error.\n");
  } else if (config_process == 0) {
    config_start();
  }
  
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
    
    /*if (request->found_get) {
      pthread_t new_thread;
      pthread_create(&new_thread, NULL, client_code, request);

    } else {
      close(client_socket);
    }*/
  }

  cleanup();

  return 0;
}

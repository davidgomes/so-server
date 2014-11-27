#include "main.h"

void cleanup() {
  printf("Cleaning up.\n");
  close(connection_socket);
  close(client_socket);
  exit(0);
}

void init() {
  request_buffer = buffer_create(20); // replace 20 with 2 * NUM_THREADS
  sem_unlink("buffer_full");
  sem_buffer_full = sem_open("buffer_full",O_CREAT|O_EXCL,0700, 20);
  sem_unlink("buffer_empty");
  sem_buffer_full = sem_open("buffer_empty",O_CREAT|O_EXCL,0700, 0);
  sem_unlink("threads");
  sem_buffer_full = sem_open("threads",O_CREAT|O_EXCL,0700, 10);

  scheduler.buffer = request_buffer;
  scheduler.sem_buffer_empty = sem_buffer_empty;
  scheduler.sem_buffer_full = sem_buffer_full;
  scheduler.sem_threads = sem_threads;
  scheduler.policy = FIFO_POLICY;

  connection_socket = connection_start(3000);
  client_name_len = sizeof(client_name);

  signal(SIGINT, cleanup);

  pid_t config_process = fork();

  if (config_process == -1) {
    fprintf(stderr, "Fork error.\n");
  } else if (config_process == 0) {
    config_start();
  }
}

int main(void) {
  init();
  
  while (true) {
    if ((client_socket = accept(connection_socket,
                                (struct sockaddr *) &client_name,
                                &client_name_len)) == -1 ) {
      printf("Error accepting connection.\n");
      exit(1);
    }

    http_request *request = (http_request*) malloc(sizeof(http_request));
    http_parse_request(client_socket, request);

    buffer_add(request_buffer, request);
    
    if (request->found_get) {
      pthread_t new_thread;
      pthread_create(&new_thread, NULL, client_code, request);

    } else {
      close(client_socket);
    }
  }

  cleanup();

  return 0;
}

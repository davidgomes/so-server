#include "main.h"

void cleanup() {
  printf("Cleaning up.\n");
  close(connection_socket);
  close(client_socket);
  exit(0);
}


int main(void) {
  request_buffer = buffer_create(20); // replace 20 with 2 * NUM_THREADS
  sem_init(&sem_buffer_full, 0, 20);
  sem_init(&sem_buffer_empty, 0, 0);
  sem_init(&sem_threads, 0, 10); //replace 10 with NUM_THREADS
  scheduler.buffer = request_buffer;
  scheduler.sem_buffer_empty = sem_buffer_empty;
  scheduler.sem_buffer_full = sem_buffer_full;
  scheduler.sem_threads = sem_threads;
  scheduler.policy = FIFO_POLICY;

  connection_socket = connection_start(3000);
  client_name_len = sizeof(client_name);

  signal(SIGINT, cleanup);

  while (1) {
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

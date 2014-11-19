#include "main.h"

void cleanup() {
  printf("Cleaning up.\n");
  close(connection_socket);
  close(client_socket);
  exit(0);
}

int main(void) {
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

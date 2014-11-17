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

    http_send_header(client_socket);

    int found_get = utils_found_get(client_socket);

    if (found_get) {
      FILE *fp;

      fp = fopen("../data/index.html", "r");

      char buf_tmp[SIZE_BUF];
      while (fgets(buf_tmp, SIZE_BUF, fp)) {
        send(client_socket, buf_tmp, strlen(buf_tmp), 0);
      }

      fclose(fp);

      close(client_socket);
    }
  }

  cleanup();

  return 0;
}

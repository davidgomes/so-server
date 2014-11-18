#include "client.h"

void *client_code(void *socket) {
  printf("A new thread has been created.\n");

  int client_socket = *((int*) socket);

  http_send_header(client_socket);
  
  FILE *fp;

  fp = fopen("../data/index.html", "r");

  char buf_tmp[SIZE_BUF];
  while (fgets(buf_tmp, SIZE_BUF, fp)) {
    send(client_socket, buf_tmp, strlen(buf_tmp), 0);
  }

  fclose(fp);
  close(client_socket);

  pthread_exit(NULL);
  return NULL;
}

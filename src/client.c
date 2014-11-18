#include "client.h"

void *client_code(void *socket) {
  int client_socket = *((int*) socket);

  printf("Hi\n");
  
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

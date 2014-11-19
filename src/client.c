#include "client.h"

void *client_code(void *socket) {
  http_request *request = ((http_request*) socket);

  http_send_header(request->socket, "text/html");

  FILE *fp;

  if (strcmp(request->name, "favicon.ico") == 0) {
    pthread_exit(NULL);
    return NULL;
  }
  
  if (!strlen(request->name)) {
    strcpy(request->name, "index.html");
  }

  char file_name[SIZE_BUF];
  sprintf(file_name, "../data/%s", request->name);
  
  fp = fopen(file_name, "r");

  char buf_tmp[SIZE_BUF];
  while (fgets(buf_tmp, SIZE_BUF, fp)) {
    send(request->socket, buf_tmp, strlen(buf_tmp), 0);
  }

  fclose(fp);
  close(request->socket);

  pthread_exit(NULL);
  return NULL;
}

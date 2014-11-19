#include "http.h"

/**
 * Parses a given HTTP request. If it is a GET, then
 * set request->found_get accordingly. Also, set request->name
 * to the name of the file wanted.
 */
void http_parse_request(int socket, http_request *request) {
  int found_get = 0;
  int i, j;

  while (utils_socket_read_line(socket, SIZE_BUF) > 0) {
    if (!strncmp(buf, GET_EXPR, strlen(GET_EXPR))) {
      found_get = 1;
      i = strlen(GET_EXPR);
      j = 0;

      while (buf[i] != ' ' && buf[i] != '\0') {
        request->name[j++] = buf[i++];
      }

      request->name[j] = '\0';
    }
  }

  request->socket = socket;
  request->found_get = found_get;
}

void http_send_header(int socket) {
  char buf[SIZE_BUF];
  
  sprintf(buf,HEADER_1);
  send(socket,buf,strlen(HEADER_1),0);

  sprintf(buf,SERVER_STRING);
  send(socket,buf,strlen(SERVER_STRING),0);

  sprintf(buf,HEADER_2);
  send(socket,buf,strlen(HEADER_2),0);
}

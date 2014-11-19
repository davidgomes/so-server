#ifndef HTTP_HEADER
#define HTTP_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "utils.h"
#include "constants.h"

#define SERVER_STRING "Server: simpleserver/0.1.0\r\n"
#define HEADER_1 "HTTP/1.0 200 OK\r\n"
#define HEADER_2 "Content-Type: text/html\r\n\r\n"

typedef struct http_request {
  int found_get;
  int socket;
  char name[SIZE_BUF];
} http_request;

void http_parse_request(int socket, http_request *request);
void http_send_header(int socket);

#endif

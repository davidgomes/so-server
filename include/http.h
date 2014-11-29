#ifndef HTTP_HEADER
#define HTTP_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "utils.h"
#include "constants.h"

#define HEADER_1 "HTTP/1.0 200 OK\r\n"
#define HEADER_2 "Content-Type: text/html\r\n\r\n"

#define STATIC_PAGE 1
#define DYNAMIC_SCRIPT 2
#define SOMETHING_ELSE 3

typedef struct http_request {
  int found_get;
  int socket;
  char name[SIZE_BUF];
  int type;
} http_request;

void http_parse_request(int socket, http_request *request);
void http_send_header(int socket, char content_type[]);

#endif

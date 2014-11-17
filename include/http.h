#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "constants.h"

#define SERVER_STRING   "Server: simpleserver/0.1.0\r\n"
#define HEADER_1        "HTTP/1.0 200 OK\r\n"
#define HEADER_2        "Content-Type: text/html\r\n\r\n"

void http_send_header(int socket);

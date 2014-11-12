#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "connection.h"

#define SERVER_STRING   "Server: simpleserver/0.1.0\r\n"
#define HEADER_1        "HTTP/1.0 200 OK\r\n"
#define HEADER_2        "Content-Type: text/html\r\n\r\n"
#define SIZE_BUF 1024

void send_header(int socket);

int connection_socket;
struct sockaddr_in client_name;
socklen_t client_name_len;
char buf[SIZE_BUF];

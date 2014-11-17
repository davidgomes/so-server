#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define GET_EXPR "GET /"
#define SIZE_BUF 1024

int utils_found_get(int socket);
int utils_socket_read_line(int socket, int n);

char buf[SIZE_BUF];

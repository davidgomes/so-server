#include <stdio.h>
#include <unistd.h>

#define SIZE_BUF 1024

int utils_socket_read_line(int socket, int n);

char buf[SIZE_BUF];

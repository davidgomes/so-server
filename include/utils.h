#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "constants.h"

#define GET_EXPR "GET /"

int utils_found_get(int socket);
int utils_socket_read_line(int socket, int n);

char buf[SIZE_BUF];

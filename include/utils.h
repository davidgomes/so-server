#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "constants.h"

#define GET_EXPR "GET /"

int utils_socket_read_line(int socket, int n);
char *utils_get_file_extension(char file_name[]);

char buf[SIZE_BUF];

#endif

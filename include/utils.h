#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "constants.h"

#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"
#define DEBUG 1

void utils_get_current_time(char *result);
int utils_socket_read_line(int socket, int n);
char *utils_get_file_extension(char file_name[]);
void utils_get_prefix_directory(char *result, char file_name[]);
void utils_debug(char to_print[]);

char buf[SIZE_BUF];

#endif

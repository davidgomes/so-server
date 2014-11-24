#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "connection.h"
#include "http.h"
#include "constants.h"
#include "client.h"
#include "buffer.h"
#include "scheduler_thread.h"

int connection_socket;
struct sockaddr_in client_name;
socklen_t client_name_len;
char buf[SIZE_BUF];
int client_socket;
buffer* request_buffer;
scheduler_data* scheduler_args;
pthread_t scheduler_thread;

sem_t sem_buffer_empty; // 0 is buffer is empty
sem_t sem_buffer_full; // 0 is buffer is full
sem_t sem_threads; // 0 is there is no available thread

#endif

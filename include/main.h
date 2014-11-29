#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "connection.h"
#include "http.h"
#include "constants.h"
#include "client.h"
#include "buffer.h"
#include "scheduler_thread.h"
#include "config.h"

int connection_socket;
struct sockaddr_in client_name;
socklen_t client_name_len;
char buf[SIZE_BUF];
int client_socket;

buffer *request_buffer;
scheduler_data *scheduler_args;
pthread_t scheduler_thread;
int thread_ready[10];
http_request *requests[10];

pthread_mutex_t thread_locks[10];
pthread_cond_t wait_for_work[10];
pthread_mutex_t cond_lock[10];
client_data workers[10];
pthread_t client_threads[10];

sem_t *sem_buffer_empty; // 0 is buffer is empty
sem_t *sem_buffer_full; // 0 is buffer is full
sem_t *sem_threads; // 0 is there is no available thread

#endif

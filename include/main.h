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
#include <sys/msg.h>
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
#include "stats.h"

int connection_socket;
struct sockaddr_in client_name;
socklen_t client_name_len;
char buf[SIZE_BUF];
int client_socket;

buffer *request_buffer;
scheduler_data *scheduler_args;
pthread_t scheduler_thread;

/*
  All of the following variables are arrays of config->n_threads sized
  allocated in heap. This approach is preferred to having, for example,
  "thread_ready[MAX_MAX_THREADS];".
*/
int *thread_ready;
http_request **requests;
pthread_mutex_t *thread_locks;
pthread_cond_t *wait_for_work;
pthread_mutex_t *cond_lock;
client_data *workers;
pthread_t *client_threads;

sem_t *sem_buffer_empty; // 0 if buffer is empty
sem_t *sem_buffer_full; // 0 if buffer is full
sem_t *sem_threads; // 0 if there is no available thread
pthread_mutex_t *buffer_mutex;

int configuration_shmid;
config_t *config;
pthread_cond_t *wait_for_config;
pthread_mutex_t *config_mutex;
config_args_t *config_args;

pid_t stats_process;
pid_t config_process;
int message_queue_id;

/* Functions*/
void main_shutdown_threads();
void main_shutdown_processes();
void main_free_thread_memory();
void main_shutdown();

void main_cleanup();

void main_init_clients();
void main_init_semaphores();
void main_init_scheduler();

void main_init_stats();
void main_init_message_queue();

void main_init_config();
void main_init_shared_memory();

void main_init();
void main_run();

void server_close();
void server_start();

int main(void);

#endif

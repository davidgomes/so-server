#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <semaphore.h>

#include "http.h"
#include "constants.h"

struct {
  int id;
  pthread_mutex_t *lock;
  int *ready;
  sem_t *sem_threads;
  http_request **request;
  pthread_cond_t *wait_for_work;
  pthread_mutex_t *cond_lock;
} typedef client_data;

void *client_code(void *which_client);
void client_serve_request(http_request *request);

#endif

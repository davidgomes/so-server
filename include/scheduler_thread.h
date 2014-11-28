#ifndef SCHEDULER_HEADER
#define SCHEDULER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

#define FIFO_POLICY 0
#define STATIC_POLICY 1
#define DYNAMIC_POLICY 2

struct{
	buffer* buffer;
	sem_t *sem_buffer_full, *sem_buffer_empty, *sem_threads;
	int policy;
  int* thread_ready;
  http_request** requests;
  pthread_mutex_t *thread_locks;
  pthread_cond_t* wait_for_work;
  pthread_mutex_t* cond_lock;

}typedef scheduler_data;

void* scheduler_code(void* data);

#endif

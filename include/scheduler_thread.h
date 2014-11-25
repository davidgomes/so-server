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

struct scheduler_data {
  sem_t *sem_buffer_full, *sem_buffer_empty, *sem_threads;

  buffer* buffer;
  int policy;
} typedef scheduler_data;

void scheduler_code(void *data);

#endif

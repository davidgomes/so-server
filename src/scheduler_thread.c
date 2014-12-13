#include "scheduler_thread.h"

void cleanup_scheduler() {
  printf("scheduler_cleanup\n");
  pthread_exit(0);
}

void* scheduler_code(void* data) {
  printf("scheduler_thread running\n");

  signal(SIGUSR2, cleanup_scheduler);

  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGUSR2);

  scheduler_data* param = (scheduler_data*) data;
  buffer* buf = param->buffer;
  sem_t *sem_buffer_empty = param->sem_buffer_empty;
  sem_t *sem_buffer_full = param->sem_buffer_full;
  pthread_mutex_t *buffer_mutex = param->buffer_mutex;

  while (true) {
    sem_wait(sem_buffer_empty); // only remove if buffer is not empty
    sigprocmask(SIG_BLOCK, &set, NULL);
    //block_signals();

    pthread_mutex_lock(buffer_mutex);
    printf("New Request.\n");
    buffer_node* node = buf->first->next;
    buffer_node* parent = buf->first;
    buffer_node* best = NULL;

    if (param->policy == FIFO_POLICY) {
      best = node;
    } else {
      while (node->next != NULL) {
        if ((param->policy == STATIC_POLICY && node->request->type == STATIC_PAGE) ||
            (param->policy == DYNAMIC_POLICY && !node->request->type == DYNAMIC_SCRIPT)) {
          best = node;
          break;
        } else {
          if (best == NULL) {
            best = node;
          }
        }

        node = node->next;
        parent = parent->next;
      }
    }

    printf("On scheduler_code: work with name: %s\n", best->request->name);

    parent->next = best->next;
    buf->cur_size--;
    pthread_mutex_unlock(buffer_mutex);
    sem_post(sem_buffer_full);

    int i;
    for (i = 0; i < param->n_threads; i++) {
      pthread_mutex_lock(&param->thread_locks[i]);

      if (param->thread_ready[i] == 1) {
        param->requests[i] = best->request;
        param->thread_ready[i] = 0;
        pthread_mutex_unlock(&param->thread_locks[i]);

        pthread_cond_broadcast(&(param->wait_for_work[i]));

        break;
      }

      pthread_mutex_unlock(&param->thread_locks[i]);
    }

    if (i >= param->n_threads) {
      printf("No worker available.\n"); // This might happen if the workers are slow, because when the request is delivered to a worker we remove the request from the buffer, leaving space for more requests, but no thread available.
      char error[] = "<!DOCTYPE html>\n <head></head>\n <body> <h2>Server error. No processing units available. </h2> </body>\n\n";
      send(best->request->socket, error, strlen(error), 0);
      close(best->request->socket);
    } else {
      printf("Delivered work to worker %d\n\n", i);
    }

    sigprocmask(SIG_UNBLOCK, &set, NULL);
  }
}

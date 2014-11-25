#include "scheduler_thread.h"

void scheduler_code(void* data) {
  scheduler_data* param = (scheduler_data*) data;
  buffer* buf = param->buffer;

  //sem_t *sem_buffer_full = param->sem_buffer_full;
  sem_t *sem_buffer_empty = param->sem_buffer_empty;
  sem_t *sem_threads = param->sem_threads;

  //int policy = param->policy;

  while (true) {
    sem_wait(sem_buffer_empty);
    buffer_node* node = buf->first->next;
    buffer_node* parent = buf->first;
    buffer_node* best = NULL;

    if (param->policy == FIFO_POLICY) {
      best = node;
    } else {
      while (node->next != NULL) {
        if ((param->policy == STATIC_POLICY && node->request->type == STATIC_REQUEST) ||
            (param->policy == DYNAMIC_POLICY && !node->request->type == DYNAMIC_REQUEST)) {
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

    parent->next = node->next;
    sem_post(sem_buffer_empty);

    sem_wait(sem_threads);
    sem_post(sem_threads);
  }
}

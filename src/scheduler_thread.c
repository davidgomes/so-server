#include "scheduler_thread.h"

void* scheduler_code(void* data){
  printf("scheduler_thread running\n");

  scheduler_data* param = (scheduler_data*) data;
  buffer* buf = param->buffer;
  sem_t *sem_buffer_full = param->sem_buffer_full, *sem_buffer_empty = param->sem_buffer_empty, *sem_threads = param->sem_threads;
  int policy = param->policy;

  while(1){
    sem_wait(sem_buffer_empty); // only remove if buffer is not empty

    printf("New Request\n");
    buffer_node* node = buf->first->next;
    buffer_node* parent = buf->first;
    buffer_node* best = NULL;
    if(param->policy == FIFO_POLICY){
      best = node;
    }else{
      for(; node->next != NULL; node = node->next, parent=parent->next){
        if((param->policy == STATIC_POLICY && node->request->type == STATIC_PAGE) || (param->policy == DYNAMIC_POLICY && !node->request->type == DYNAMIC_SCRIPT)){
          best = node;
          break;
        }else{
          if(best == NULL)
            best = node;
        }
      }

    }

    printf("On scheduler_code: work with name: %s\n", best->request->name);

    parent->next = node->next;

    int i;
    for(i=0; i<10; i++){
      pthread_mutex_lock(&param->thread_locks[i]);
      if(param->thread_ready[i] == 1){
        param->requests[i] = best->request;
        param->thread_ready[i] = 0;
        pthread_mutex_unlock(&param->thread_locks[i]);
        
        //printf("On scheduler_code: Got request\n");
        
        pthread_cond_broadcast(param->wait_for_work);
        
        break;
      }
      pthread_mutex_unlock(&param->thread_locks[i]);
      
    }
    printf("Delivered work to worker %d\n\n", i);

  }

}  

#include "scheduler_thread.h"

void* scheduler_code(void* data){
  printf("scheduler_thread running\n");

  scheduler_data* param = (scheduler_data*) data;
  buffer* buf = param->buffer;
  sem_t *sem_buffer_full = param->sem_buffer_full, *sem_buffer_empty = param->sem_buffer_empty, *sem_threads = param->sem_threads;
  int policy = param->policy;

  while(1){
    printf("scheduler_thread\n");
    sem_wait(sem_buffer_empty); // only remove if buffer is not empty

    printf("New Request\n");
    buffer_node* node = buf->first->next;
    buffer_node* parent = buf->first;
    buffer_node* best = NULL;
    if(param->policy == FIFO_POLICY){
      best = node;
    }else{
      for(; node->next != NULL; node = node->next, parent=parent->next){
        if((param->policy == STATIC_POLICY && node->request->type == STATIC_REQUEST) || (param->policy == DYNAMIC_POLICY && !node->request->type == DYNAMIC_REQUEST)){
          best = node;
          break;
        }else{
          if(best == NULL)
            best = node;
        }
      }

    }

    printf("name: %s\n", best->request->name);

    parent->next = node->next;

    int i;
    for(i=0; i<10; i++){
      pthread_mutex_lock(&param->thread_locks[i]);
      if(param->thread_ready[i]){
        param->thread_ready[i] = 0;
        pthread_cond_broadcast(param->wait_for_work);
        pthread_mutex_unlock(&param->thread_locks[i]);
        break;
      }
      pthread_mutex_unlock(&param->thread_locks[i]);
      
    }
    printf("Delivered work\n");


    //sem_post(sem_buffer_empty);
    
    // we could use pipes to send work to each thread
    //sem_wait(sem_threads);
    //sem_post(sem_buffer_full);

  }  

}

#include "client.h"

void *client_code(void* cd) {
  
  client_data* data = (client_data*) cd;

  printf("Thread %d started.\n", data->id);

  while(1){
    pthread_cond_wait(data->wait_for_work, data->cond_lock);
    printf("On client_code %d: Received work\n", data->id);

    sem_wait(data->sem_threads);
    
    //printf("On client: Request Name: %s\n", (*(data->request))->name);
    client_serve_request(*(data->request) );
    sleep(10);
    
    pthread_mutex_lock(data->lock);
    *(data->ready) = 1;
    pthread_mutex_unlock(data->lock);

    sem_post(data->sem_threads);
    printf("On client_code %d: Finished work\n\n", data->id);
  }

  return NULL;

}

void client_serve_request(http_request* request){

  http_send_header(request->socket, "text/html");

  FILE *fp;

  if (strcmp(request->name, "favicon.ico") == 0) {
    return;
  }
  
  if (!strlen(request->name)) {
    strcpy(request->name, "index.html");
  }

  char file_name[SIZE_BUF];
  sprintf(file_name, "../data/%s", request->name);
  
  fp = fopen(file_name, "r");
  
  if(fp == NULL)
    return;

  char buf_tmp[SIZE_BUF];
  while (fgets(buf_tmp, SIZE_BUF, fp)) {
    send(request->socket, buf_tmp, strlen(buf_tmp), 0);
  }

  fclose(fp);
  close(request->socket);
  
}
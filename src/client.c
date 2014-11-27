#include "client.h"

void *client_code(void* cd) {
  
  client_data* data = (client_data*) cd;
  http_request inc_request;

  printf("Thread %d started.\n", data->id);

  while(1){
    pthread_cond_wait(data->wait_for_work, data->cond_lock);
    printf("On client_code: Received work\n");


    sem_wait(data->sem_threads);
    pthread_mutex_lock(data->lock);


    printf("%s\n", inc_request.name);


    *(data->ready) = 1;
    pthread_mutex_unlock(data->lock);
    sem_post(data->sem_threads);
  }


  /*void *socket;
  http_request *request = ((http_request*) socket);

  http_send_header(request->socket, "text/html");

  FILE *fp;

  if (strcmp(request->name, "favicon.ico") == 0) {
    pthread_exit(NULL);
    return NULL;
  }
  
  if (!strlen(request->name)) {
    strcpy(request->name, "index.html");
  }

  char file_name[SIZE_BUF];
  sprintf(file_name, "../data/%s", request->name);
  
  fp = fopen(file_name, "r");

  char buf_tmp[SIZE_BUF];
  while (fgets(buf_tmp, SIZE_BUF, fp)) {
    send(request->socket, buf_tmp, strlen(buf_tmp), 0);
  }

  fclose(fp);
  close(request->socket);

  pthread_exit(NULL);*/
  return NULL;
}

void client_serve_request(http_request* request){

}
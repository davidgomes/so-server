#include "client.h"

void cleanup_client() {
  printf("client_cleanup\n");
  pthread_exit(0);
}

void *client_code(void* cd) {
  signal(SIGUSR1, cleanup_client);

  client_data* data = (client_data*) cd;

  printf("Thread %d started.\n", data->id);

  while (true) {
    pthread_cond_wait(data->wait_for_work, data->cond_lock);
    printf("Cond var released\n");
    printf("On client_code %d: Received work.\n", data->id);

    sem_wait(data->sem_threads);

    client_serve_request(*(data->request));

    /* Update statistics' message queue */
    stats_message *message = (stats_message*) malloc(sizeof(stats_message));
    message->mtype = 1;
    message->thread_index = data->id;
    strcpy(message->file_name, (*(data->request))->name);

    http_int_to_type(message->request_type, (*(data->request))->type);
    
    msgsnd((*(data->request))->message_queue_id, message,
           sizeof(stats_message), 0);

    pthread_mutex_lock(data->lock);
    *(data->ready) = 1;
    pthread_mutex_unlock(data->lock);

    sem_post(data->sem_threads);
    //printf("On client_code %d: Finished work.\n", data->id);
  }

  return NULL;
}

void client_serve_request(http_request* request) {
  FILE *fp;

  if (request->type == SOMETHING_ELSE) {
    return;
  }

  char file_name[SIZE_BUF];
  sprintf(file_name, "../data/%s", request->name);

  fp = fopen(file_name, "r");

  if (fp == NULL) {
    return;
  }

  if (fp == NULL) {
    printf("File doesn't exist.\n");
    close(request->socket);
  }

  http_send_header(request->socket, "text/html");

  /*
   * Execute a script and get output back from a file passed with a pipe.
   */
  if (request->type == DYNAMIC_SCRIPT) {
    FILE *pipe_output = popen(file_name, "r");

    char output_buffer[SIZE_BUF];
    char style_string[] = "<body>";
    char style_string2[] = "</body>";
    send(request->socket, style_string, sizeof(style_string), 0);
    while (fgets(output_buffer, sizeof(output_buffer), pipe_output) != NULL) {
      send(request->socket, "<p>", 3, 0);
      send(request->socket, output_buffer, strlen(output_buffer), 0);
      send(request->socket, "</p>", 4, 0);
    }

    send(request->socket, style_string2, sizeof(style_string2), 0);

    pclose(pipe_output);
  } else if (request->type == STATIC_PAGE) {
    char buf_tmp[SIZE_BUF];

    while (fgets(buf_tmp, SIZE_BUF, fp)) {
      send(request->socket, buf_tmp, strlen(buf_tmp), 0);
    }
  }

  fclose(fp);
  close(request->socket);
}

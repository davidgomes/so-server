#include "stats.h"

void stats_start(int _message_queue_id, char log_file_name[]) {
  utils_get_current_time(start_time_str);

  signal(SIGINT, stats_close);
  //signal(SIGHUP, stats_close);

  sigemptyset(&mask);
  sigaddset(&mask, SIGTSTP);
  sigprocmask(SIG_BLOCK, &mask, NULL);

  message_queue_id = _message_queue_id;

  strcpy(config_log_file_name, log_file_name);
  
  static_requests = 0;
  dynamic_requests = 0;

  stats_loop();
}

void stats_loop() {
  stats_message *new_message = (stats_message*) malloc(sizeof(stats_message));

  while (true) {
    utils_debug("Listening for new messages in the message queue.\n");
    msgrcv(message_queue_id, new_message, sizeof(stats_message), 1, 0);

    utils_debug("Got a new message on the message queue.\n");
    
    log_file = fopen(config_log_file_name, "a");

    char time_str[MAX_TIME_STR];
    utils_get_current_time(time_str);

    if (!strcmp(new_message->request_type, "STATIC_PAGE")) {
      static_requests++;
    } else if (!strcmp(new_message->request_type, "DYNAMIC_SCRIPT")) {
      printf("hi\n");
      dynamic_requests++;
    }

    fprintf(log_file, "%s %s %s %d\n", new_message->request_type,
            time_str, new_message->file_name, new_message->thread_index);

    fclose(log_file);
  }
}

void stats_close() {
  /* Write closing logs */
  log_file = fopen(config_log_file_name, "a");

  printf("Stats close\n");
  fprintf(log_file, "Started at %s\n", start_time_str);

  char stop_time_str[MAX_TIME_STR];
  utils_get_current_time(stop_time_str);
  fprintf(log_file, "Closed at %s\n", stop_time_str);
  fprintf(log_file, "Static Requests %d\n", static_requests);
  fprintf(log_file, "Dynamic Requests %d\n", dynamic_requests);

  fclose(log_file);
  exit(0);
}

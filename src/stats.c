#include "stats.h"

void stats_start(int _message_queue_id, char log_file_name[]) {
  utils_get_current_time(start_time_str);
  
  signal(SIGINT, stats_close);
  signal(SIGHUP, stats_close);

  message_queue_id = _message_queue_id;
  log_file = fopen(log_file_name, "a");

  static_requests = 0;
  dynamic_requests = 0;
  
  stats_loop();
}

void stats_loop() {  
  stats_message *new_message = (stats_message*) malloc(sizeof(stats_message));

  while (true) {
    msgrcv(message_queue_id, new_message, sizeof(stats_message), 1, 0);

    char time_str[MAX_TIME_STR];
    utils_get_current_time(time_str);

    if (!strcmp(new_message->request_type, "STATIC_PAGE")) {
      static_requests++;
    } else if (!strcmp(new_message->request_type, "DYNAMIC_SCRIPT")) {
      dynamic_requests++;
    }
    
    fprintf(log_file, "%s %s %s %d\n", new_message->request_type,
            time_str, new_message->file_name, new_message->thread_index);
  }  
}

void stats_close() {
  /* Write closing logs */
  fprintf(log_file, "Started at %s\n", start_time_str);

  char stop_time_str[MAX_TIME_STR];
  utils_get_current_time(stop_time_str);
  fprintf(log_file, "Closed at %s\n", stop_time_str);
  fprintf(log_file, "Static Requests %d\n", static_requests);
  fprintf(log_file, "Dynamic Requests %d\n", dynamic_requests);
  
  fclose(log_file);
  exit(0);
}

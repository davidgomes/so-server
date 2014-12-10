#include "stats.h"

void stats_start(int _message_queue_id, char log_file_name[]) {
  signal(SIGINT, stats_close);

  message_queue_id = _message_queue_id;
  log_file = fopen(log_file_name, "a");

  stats_message *new_message = (stats_message*) malloc(sizeof(stats_message));
  while (true) {
    msgrcv(message_queue_id, new_message, sizeof(stats_message), 1, 0);

    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char *time_str = asctime(timeinfo);
    time_str[(int) strlen(time_str) - 1] = 0;
    fprintf(log_file, "%s %s\n", time_str, new_message->file_name);
  }
}

void stats_close() {
  fclose(log_file);
  exit(0);
}

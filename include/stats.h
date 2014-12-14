#ifndef STATS_HEADER
#define STATS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <signal.h>

#include "utils.h"
#include "constants.h"

typedef struct {
  long mtype;
  int thread_index;
  char request_type[24];
  char file_name[SIZE_BUF];
} stats_message;

char start_time_str[MAX_TIME_STR];
int static_requests;
int dynamic_requests;

FILE *log_file;
int message_queue_id;

sigset_t mask;

char config_log_file_name[100];

void stats_start(int _message_queue_id, char log_file_name[]);
void stats_loop();
void stats_close();

#endif

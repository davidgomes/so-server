#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <time.h>

#include "constants.h"

typedef struct {
  long mtype;
  char file_name[SIZE_BUF];
} stats_message;

FILE *log_file;
int message_queue_id;

void stats_start(int _message_queue_id, char log_file_name[]);
void stats_close();

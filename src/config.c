#include "config.h"

void config_sighup_handler() {
  printf("RECEIVED SIGHUP\n");
  config_read();
}

void config_ignore_tstp() { }

void config_shutdown() {
  exit(0);
}

int config_sort_type_str_to_int(char config_type_str[]) {
  if (!strcmp(config_type_str, "FIFO_POLICY")) {
    return FIFO_POLICY;
  } else if (!strcmp(config_type_str, "STATIC_POLICY")) {
    return STATIC_POLICY;
  } else if (!strcmp(config_type_str, "DYNAMIC_POLICY")) {
    return DYNAMIC_POLICY;
  } else {
    fprintf(stderr, "An error occured while reading the configuration.");
    return -1;
  }
}

void config_read() {
  FILE *config_file = fopen("../data/config.txt", "r");

  int n_threads;
  fscanf(config_file, "%d", &n_threads);
  g_config->n_threads = n_threads;
  printf("CONFIG: Read number of threads %d\n", n_threads);

  int port;
  fscanf(config_file, "%d", &port);
  g_config->port = port;

  char sort_type[SORT_TYPE_STR];
  fscanf(config_file, "%s", sort_type);
  g_config->policy_type = config_sort_type_str_to_int(sort_type);
  
  fclose(config_file);
  printf("Done Reading Configuration\n");
}

void config_start(config_t *config) {
  g_config = config;
  config_read();

  signal(SIGHUP, config_sighup_handler);
  signal(SIGTSTP, config_ignore_tstp);
  signal(SIGINT, config_shutdown);

  sigemptyset(&mask);
  sigaddset(&mask, SIGHUP);

  while (true) {
    sigsuspend(&mask);
  }

  printf("Exiting\n");
}

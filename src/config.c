#include "config.h"

void config_read() {
  FILE *config_file = fopen("../data/config.txt", "r");

  int n_threads;
  fscanf(config_file, "%d", &n_threads);
  g_config->n_threads = n_threads;
  printf("CONFIG: Read number of threads %d\n", n_threads);

  int port;
  fscanf(config_file, "%d", &port);
  g_config->port = port;

  fclose(config_file);
  printf("Read Configuration\n");
}

void sighandler() {
  printf("RECEIVED SIGHUP\n");
  config_read();
}

void ignorestp() {
  //printf("IGNORING\n");
}

void config_shutdown() {
  exit(0);
}

void config_start(config_t *config) {
  g_config = config;
  config_read();

  signal(SIGHUP, sighandler);
  signal(SIGTSTP, ignorestp);
  signal(SIGINT, config_shutdown);

  sigemptyset(&mask);
  sigaddset(&mask, SIGHUP);

  while (true) {
    sigsuspend(&mask);
  }

  printf("Exiting\n");
}

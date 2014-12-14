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

  pthread_cond_broadcast(wait_for_config);
  printf("Read Configuration\n");
}

void config_sighup_handler() {
  printf("RECEIVED SIGHUP\n");
  config_read();
}

void config_shutdown() {
  exit(0);
}

void config_start(config_args_t *config_args) {
  g_config = &config_args->config;
  wait_for_config = &config_args->wait_for_config;
  config_mutex = &config_args->config_mutex;
  
  config_read();

  signal(SIGHUP, config_sighup_handler);
  signal(SIGINT, config_shutdown);

  sigemptyset(&mask);
  sigaddset(&mask, SIGTSTP);
  sigprocmask(SIG_BLOCK, &mask, NULL);

  printf("Config will now wait\n");
  while (true) {
    sigsuspend(&mask);
  }

  printf("Exiting\n");
}

#include "config.h"

void config_start(config_t *config) {
  FILE *config_file = fopen("../data/config.txt", "r");

  int n_threads;
  fscanf(config_file, "%d", &n_threads);
  config->n_threads = n_threads;

  int port;
  fscanf(config_file, "%d", &port);
  config->port = port;

  fclose(config_file);
}

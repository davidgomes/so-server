#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include <stdio.h>

struct {
  int n_threads;
  int port;
} typedef config_t;

void config_start(config_t *config);

#endif

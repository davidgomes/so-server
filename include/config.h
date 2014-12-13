#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "constants.h"

struct {
  int n_threads;
  int port;
} typedef config_t;

config_t *g_config;
sigset_t mask;

void config_start(config_t *config);
void config_read();

#endif

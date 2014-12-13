#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "constants.h"

struct {
  int n_threads;
  int port;
  int policy_type;
} typedef config_t;

config_t *g_config;
sigset_t mask;

void config_sighup_handler();
void config_ignore_tstp();
void config_shutdown();
int config_sort_type_str_to_int(char config_type_str[]);

void config_start(config_t *config);
void config_read();

#endif

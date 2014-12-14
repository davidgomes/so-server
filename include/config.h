#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

#include "constants.h"

struct {
  int n_threads;
  int port;
  int policy_type;
} typedef config_t;

struct {
  config_t config;
  pthread_cond_t wait_for_config;
  pthread_mutex_t config_mutex;
} typedef config_args_t;

config_t *g_config;
pthread_cond_t *wait_for_config;
pthread_mutex_t *config_mutex;
sigset_t mask;

void config_sighup_handler();
void config_shutdown();
void config_start(config_args_t *config_args);
int config_sort_type_str_to_int(char config_type_str[]);
void config_read();

#endif

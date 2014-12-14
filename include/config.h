#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "constants.h"

struct {
  int n_threads;
  int port;
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

void config_start(config_args_t *config_args);
void config_read();

#endif

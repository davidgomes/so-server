#include "config.h"

void config_read() {

  sigset_t mask2;
  sigemptyset(&mask2);
  sigaddset(&mask2, SIGHUP);
  sigprocmask(SIG_BLOCK, &mask2, NULL);

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

  int n_scripts;
  fscanf(config_file, "%d", &n_scripts);
  g_config->n_scripts = n_scripts;

  char debug_str[100];
  sprintf(debug_str, "There are %d allowed scripts in the configuration.\n", n_scripts);
  utils_debug(debug_str);

  int i;
  for (i = 0; i < n_scripts; i++) {
    char str[100];
    fscanf(config_file, "%s", str);
    strcpy(g_config->scripts[i], str);
  }

  fclose(config_file);

  //pthread_cond_broadcast(wait_for_config);
  pthread_mutex_unlock(config_mutex); //solves racing problem
  printf("Read Configuration\n");
  sigprocmask(SIG_UNBLOCK, &mask2, NULL);
}

void config_sighup_handler() {
  printf("RECEIVED SIGHUP\n");
  config_read();
}

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

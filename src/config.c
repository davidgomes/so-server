#include "config.h"

void config_start() {
  FILE *config_file = fopen("config.txt", "r");

  int n_threads;
  fscanf(config_file, "%d", &n_threads);

  printf("Number of workers: %d.\n", n_threads);

  // TODO Michel escrever n_threads para uma variavel no main.h
  // com recurso a memoria partilhada
  
  fclose(config_file);
}

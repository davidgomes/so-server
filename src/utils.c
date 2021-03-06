#include "utils.h"

/* Gets current asctime without newline */
void utils_get_current_time(char *result) {
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strcpy(result, asctime(timeinfo));
  result[(int) strlen(result) - 1] = 0;
}

int utils_socket_read_line(int socket, int n) {
  int n_read;
  int not_eol;
  int ret;
  char new_char;

  n_read = 0;
  not_eol = 1;

  while (n_read < n && not_eol) {
    ret = read(socket, &new_char, sizeof(char));

    if (ret == -1) {
      printf("Error reading from socket (read_line): %s\n", strerror(errno));
      return -1;
    } else if (ret == 0) {
      return 0;
    } else if (new_char == '\r') {
      not_eol = 0;
      // consumes next byte on buffer (LF)
      read(socket, &new_char, sizeof(char));
      continue;
    } else {
      buf[n_read] = new_char;
      n_read++;
    }
  }

  buf[n_read] = '\0';

  return n_read;
}

char *utils_get_file_extension(char file_name[]) {
  char *dot = strchr(file_name, '.');

  if (!dot || dot == file_name) {
    return NULL;
  }

  return dot + 1;
}

void utils_get_prefix_directory(char *result, char file_name[]) {
  char *prefix = file_name;
  int i = 0;

  char *slash = strchr(file_name, '/');

  if (!slash || slash == file_name) {
    result = "";
    return;
  }
  
  while (*prefix != '/') {
    result[i++] = *(prefix++);
  }
}

void utils_debug(char to_print[]) {
  #ifdef DEBUG
  printf("%s%s", KRED, to_print);
  printf("%s", KWHT);
  #endif
}

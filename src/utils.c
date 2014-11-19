#include "utils.h"

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

#include "main.h"

int main(void) {
  int client_socket;
  
  connection_socket = start_connection(3000);
  client_name_len = sizeof(client_name);
  
  while (1) {
    if ((client_socket = accept(connection_socket, (struct sockaddr *) &client_name, &client_name_len)) == -1 ) {
      printf("Error accepting connection.\n");
      exit(1);
    }
  }

  return 0;
}

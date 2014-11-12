#include "connection.h"

int start_connection(int port) {
  int my_socket;
  struct sockaddr_in name;

  my_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (my_socket == -1) {
    printf("Error creating socket.");
  }

  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(my_socket, (struct sockaddr *) &name, sizeof(name)) < 0) {
    printf("Error binding to socket\n");
  }

  // Starts listening on socket
  if (listen(my_socket, 5) < 0) {
    printf("Error listening to socket\n");
  }

  return my_socket;
}

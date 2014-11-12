#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "connection.h"

int connection_socket;
struct sockaddr_in client_name;
socklen_t client_name_len;

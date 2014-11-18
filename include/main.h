#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "connection.h"
#include "utils.h"
#include "http.h"
#include "constants.h"
#include "client.h"

int connection_socket;
struct sockaddr_in client_name;
socklen_t client_name_len;
char buf[SIZE_BUF];
int client_socket;

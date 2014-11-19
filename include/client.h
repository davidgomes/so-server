#ifndef CLIENT_HEADER
#define CLIENT_HEADER

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "http.h"
#include "constants.h"

void *client_code(void *which_client);

#endif

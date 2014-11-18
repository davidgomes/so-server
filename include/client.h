#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "constants.h"

void *client_code(void *which_client);

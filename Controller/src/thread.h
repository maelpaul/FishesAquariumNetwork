#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "server_utils.h"

#define REFRESH_TIME 5

struct client_arg {
    int newsockfd;
    struct aquarium * aquarium;
};

int client(void * client_arg);
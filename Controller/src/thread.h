#ifndef _THREAD_H_
#define _THREAD_H_

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

struct client_args {
    int *client_id;
    struct aquarium * aquarium;
    int client_number;
    struct view * client_view;
    int thread_number;
};

int client(void * client_arg);

#endif
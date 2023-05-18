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
#include <assert.h>

#include "thread.h"
#include "server_utils.h"

int add_fish_server(char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int sock);
int del_fish_server(char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int sock);
int start_fish_server(char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int sock);
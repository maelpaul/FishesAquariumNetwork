#ifndef _COMMAND_GET_FISH_H_
#define _COMMAND_GET_FISH_H_

#include "server_utils.h"
#include <pthread.h>

struct for_thread {
    pthread_mutex_t * mutex;
    struct aquarium * aquarium;
    char * header;
    int client_id;
    int check_ls;
    int client_number;
    struct view * client_view;
};

int get_fish_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, struct view * client_view);

int get_fish_ls_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, struct view * client_view);

int get_fish_continuously_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, int buffer_size, struct view * client_view);

int get_status_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, struct view * client_view);

int ping_server(int check_ls, int client_number, char * header, char * buffer, int client_id);

#endif
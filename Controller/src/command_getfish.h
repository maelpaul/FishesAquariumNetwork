#ifndef _COMMAND_GET_FISH_H_
#define _COMMAND_GET_FISH_H_

#include "server_utils.h"

int get_fish_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id);

int get_fish_continuously_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id);

int get_status_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id);

int ping_server(int check_ls, int client_number, char * header, char * buffer, int client_id);

#endif
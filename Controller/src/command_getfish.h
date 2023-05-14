#include "server_utils.h"

int get_fish_server(char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id);

int get_fish_continuously_server(char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id);

int get_status_server(char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id);

int ping_server(char * buffer, int client_id);

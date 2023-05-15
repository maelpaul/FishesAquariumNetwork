#include "server_utils.h"

void prompt_load(char * buffer, struct aquarium * aquarium, int client_id, 
                int * aquarium_flag, pthread_mutex_t * mutex_aquarium);

int prompt_add_view(char * buffer, struct aquarium * aquarium, int client_id);

int prompt_del_view(char * buffer, struct aquarium * aquarium, int client_id);

int prompt_save(char * buffer, struct aquarium * aquarium, int client_id);

int prompt_show(char * buffer, struct aquarium * aquarium, int client_id);
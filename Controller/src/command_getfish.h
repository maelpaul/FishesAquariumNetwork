#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server_utils.h"

int get_fish_server(char * buffer, struct aquarium * aquarium, int client_id);

int get_fish_continuously_server(char * buffer, struct aquarium * aquarium, int client_id);
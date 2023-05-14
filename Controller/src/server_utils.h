#ifndef _SERVER_UTILS_H_
#define _SERVER_UTILS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aquarium.h"
#include "view.h"
#include "fish.h"

#define REFRESH_TIME 5

struct config {
    int controller_port;
    int display_timeout_value;
    int fish_update_interval;
};

int load_config(const char *filename, struct config *conf);

int load_initial_aquarium_config(const char *filename, struct aquarium *aquarium);

void save_aquarium(struct aquarium * aquarium);

int controller_add_fish(struct aquarium *aquarium, int * coords, int * size, char * name, void (*path)(struct fish *, int, int));

int controller_del_fish(struct aquarium *aquarium, char *fish);

char * client_connection(struct aquarium *aquarium, char* view_name);

char * find_and_attibute_free_view(struct aquarium *aquarium);

int hello_command_check(char * buffer, char * view_name);

char * get_fish(struct aquarium *aquarium, char * fish_name);

void controller_update_fishes(struct aquarium * aquarium, int refresh_time);

int controller_start_fish(struct aquarium * aquarium, char * fish_name, int time_to_dest);

void controller_aquarium_print(struct aquarium * aquarium, char * to_print);

int verif(char * buf, char * s, int client_id, pthread_mutex_t * mutex_client_count);

#endif

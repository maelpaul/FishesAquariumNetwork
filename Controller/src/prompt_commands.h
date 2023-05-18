#ifndef _PROMPT_COMMANDS_H_
#define _PROMPT_COMMANDS_H_


#include "server_utils.h"
#include "parser.h"

void prompt_load(struct command * command, struct aquarium * aquarium, 
                int * aquarium_flag, pthread_mutex_t * mutex_aquarium, 
                pthread_mutex_t * mutex_aquarium_flag);

void prompt_add_view(struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium);

void prompt_del_view(struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium);

void prompt_save(struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium);

void prompt_show(struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium);

void clear(char* str, int size);

#endif
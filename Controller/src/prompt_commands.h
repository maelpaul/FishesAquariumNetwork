#ifndef _PROMPT_COMMANDS_H_
#define _PROMPT_COMMANDS_H_

#include <dirent.h>
#include "server_utils.h"
#include "parser.h"

void prompt_load(int check_log, struct command * command, struct aquarium * aquarium, 
                int * aquarium_flag, pthread_mutex_t * mutex_aquarium, 
                pthread_mutex_t * mutex_aquarium_flag);

void prompt_add_view(int check_log, struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium);

void prompt_del_view(int check_log, struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium);

void prompt_save(int check_log, struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium);

void prompt_show(int check_log, struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium);

void clear(char* str, int size);

#endif
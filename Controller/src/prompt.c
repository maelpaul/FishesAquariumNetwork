#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "prompt.h"

#define BUFFER_SIZE 256
#define MAX_ARG 4

void prompt(int * aquarium_flag, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium)
{
    char buffer[BUFFER_SIZE];
    char args[MAX_ARG][BUFFER_SIZE];

    printf("Enter your command :\n");
    
    int should_continue = 1;
    while (should_continue) {
        struct command * command = malloc(sizeof(struct command));
    
        init_command(command);
        clear(buffer, BUFFER_SIZE);
        for (int i = 0; i < MAX_ARG; ++i)
            clear(args[i], BUFFER_SIZE);
        
        ssize_t read_size = read(0, buffer, BUFFER_SIZE);
        if (read_size == -1) {
            perror("read");
            free_command(command);
        }
        
        int arg_count = 0;
        int arg_index = 0;
        int in_word = 0;
        
        for (int i = 0; i < read_size; i++) {
            if (buffer[i] == ' ' || buffer[i] == '\n') {
                if (in_word) {
                    args[arg_count][arg_index] = '\0';
                    arg_count++;
                    arg_index = 0;
                    in_word = 0;

                    if (arg_count >= MAX_ARG) {
                        break;
                    }
                }
            } else {
                args[arg_count][arg_index] = buffer[i];
                arg_index++;
                in_word = 1;
            }
        }

        if (in_word) {
            args[arg_count][arg_index] = '\0';
            arg_count++;
        }

        char* my_argv[MAX_ARG];
        for (int i = 0; i < arg_count; i++) {
            my_argv[i] = args[i];
        }

        if(parse_command(command, my_argv, arg_count)){
            if(!*aquarium_flag){
                if(strcmp(command->command_name,"load")==0){
                    prompt_load(command, aquarium, aquarium_flag, mutex_aquarium);
                }
                else{
                    printf("> You first need to load an aquarium\n");
                }
            }
            else{
                if(strcmp(command->command_name,"add view")==0){
                    prompt_add_view(command, aquarium);
                }
                else if(strcmp(command->command_name,"del view")==0){
                    prompt_del_view(command, aquarium);
                }
                else if(strcmp(command->command_name,"show")==0){
                    prompt_show(command, aquarium);
                }
                else if(strcmp(command->command_name,"save")==0){
                    prompt_save(command, aquarium);   
                }
            }
        }
        
        if (strcmp(buffer, "> Bye") == 0) {
            should_continue = 0;
        }
            
        free_command(command);
    }
}

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

void prompt(int check, int * aquarium_flag, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium, pthread_mutex_t * mutex_aquarium_flag)
{
    char buffer[BUFFER_SIZE];
    char args[MAX_ARG][BUFFER_SIZE];

    printf("Enter your command :\n");
    write_in_log(check, "print", 0, 0, "Enter your command :\n");
    
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
        for (int i = 0; i < MAX_ARG; i++) {
            my_argv[i] = "";
        }
        char buf[200];

        for (int i = 0; i < arg_count; i++) {
            my_argv[i] = args[i];
        }

        strcpy(buf, my_argv[0]);
        for (int i = 1; i < arg_count; i++) {
            strcat(buf, " ");
            strcat(buf, my_argv[i]);
        }
        write_in_log(check, "prompt_send", 0, 0, buf);

        if(parse_command(check, command, my_argv, arg_count)){
            if (strcmp(command->command_name,"exit")==0) {
                free_command(command);
                return;
            }
            pthread_mutex_lock(mutex_aquarium_flag);
            if(!*aquarium_flag){
                pthread_mutex_unlock(mutex_aquarium_flag);
                if(strcmp(command->command_name,"load")==0){
                    prompt_load(check, command, aquarium, aquarium_flag, mutex_aquarium, mutex_aquarium_flag);
                }
                else{
                    printf("> You first need to load an aquarium\n");
                    write_in_log(check, "print", 0, 0, "> You first need to load an aquarium\n");
                }
            }
            else{
                pthread_mutex_unlock(mutex_aquarium_flag);
                if(strcmp(command->command_name,"load")==0){
                    prompt_load(check, command, aquarium, aquarium_flag, mutex_aquarium, mutex_aquarium_flag);
                }
                else if(strcmp(command->command_name,"add view")==0){
                    prompt_add_view(check, command, aquarium, mutex_aquarium);
                }
                else if(strcmp(command->command_name,"del view")==0){
                    prompt_del_view(check, command, aquarium, mutex_aquarium);
                }
                else if(strcmp(command->command_name,"show")==0){
                    prompt_show(check, command, aquarium, mutex_aquarium);
                }
                else if(strcmp(command->command_name,"save")==0){
                    prompt_save(check, command, aquarium, mutex_aquarium);   
                }
            }
        }
        
        if (strcmp(buffer, "> Bye") == 0) {
            should_continue = 0;
        }
            
        free_command(command);
    }
}

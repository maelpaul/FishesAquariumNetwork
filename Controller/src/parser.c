#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define COMMAND_LOAD_START_ARG 1
#define COMMAND_SHOW_START_ARG 1
#define COMMAND_ADD_VIEW_START_ARG 2
#define COMMAND_DEL_VIEW_START_ARG 2
#define COMMAND_SAVE_START_ARG 1

#define COMMAND_LOAD_ARG_SIZE 1
#define COMMAND_SHOW_ARG_SIZE 1
#define COMMAND_ADD_VIEW_ARG_SIZE 2
#define COMMAND_DEL_VIEW_ARG_SIZE 1
#define COMMAND_SAVE_ARG_SIZE 1

void init_command(struct command * command){
    command->command_name = NULL;
    command->nb_params = 0;
    command->params = NULL;
    command->_total_param_len = 0;
}

int check_command(struct command * command, char** argv, int argc, char * command_name, int start_arg, int arg_size){
    if (argc == start_arg + arg_size){
        command->command_name = command_name;
        command->nb_params = arg_size;

        int malloc_size = command->nb_params * sizeof(char*);
        command->params = malloc(malloc_size);

        // Copy args
        for (int i = 0; i < arg_size; ++i){
            
            command->params[i] = argv[start_arg + i];
        }

        command->_total_param_len = malloc_size;

        return 1;
    }

    return 0;
}

int parse_command(int check, struct command * command, char** argv, int argc){
    int result;

    if(strcmp(argv[0], "exit") == 0){
        command->command_name = "exit";
        return 1;
    }
    else if(strcmp(argv[0], "help") == 0){
        printf("> You can use \"load\", \"save\", \"show\", \"add view\", \"del view\" or \"exit\" command\n");
        write_in_log(check, "print", 0, 0, "> You can use \"load\", \"save\", \"show\", \"add view\", \"del view\" or \"exit\" command\n");
        return 0;

    } else if(strcmp(argv[0], "load") == 0){
        result = check_command(command, argv, argc, "load", COMMAND_LOAD_START_ARG, COMMAND_LOAD_ARG_SIZE);
        if (!result){
            printf("> Incorrect use of \"load\", there should be 1 argument (received %d) which is the aquarium to load.\n", argc - 1);
            write_in_log(check, "print", 0, 0, "> Incorrect use of \"load\"\n");}
        return result;

    } else if(strcmp(argv[0], "show") == 0){
        result = check_command(command, argv, argc, "show", COMMAND_SHOW_START_ARG, COMMAND_SHOW_ARG_SIZE);
        if (!result){
            printf("> Incorrect use of \"show\", there should be 1 argument (received %d) which is the aquarium to show.\n", argc-1);
            write_in_log(check, "print", 0, 0, "> Incorrect use of \"show\"\n");}
        return result;

    } else if (strcmp(argv[0], "add") == 0 && strcmp(argv[1], "view") == 0){
        if(!check_add_wiew_format(argv[3])){
            printf("> Incorrect use of \"add view\", the second argument should respect the following format :" 
                "VIEW_X x VIEW_Y + VIEW_WITDH + VIEW_HEIGHT without spaces (received %s)\n",argv[3]);
            write_in_log(check, "print", 0, 0, "> Incorrect use of \"add view\"\n");
            return 0;
        };

        result = check_command(command, argv, argc, "add view", COMMAND_ADD_VIEW_START_ARG, COMMAND_ADD_VIEW_ARG_SIZE);
        if (!result){
            printf("> Incorrect use of \"add view\", there should be 2 arguments (received %d) which are the name of the view and its size\n", argc - 2);
            write_in_log(check, "print", 0, 0, "> Incorrect use of \"add view\"\n");}

        return result;
    } else if(strcmp(argv[0], "del") == 0 && strcmp(argv[1], "view") == 0){
        result = check_command(command, argv, argc, "del view", COMMAND_DEL_VIEW_START_ARG, COMMAND_DEL_VIEW_ARG_SIZE);
        if (!result){
            printf("> Incorrect use of \"del view\", there should be 1 argument (received %d) which is the name of the view to remove.\n", argc - 2);
            write_in_log(check, "print", 0, 0, "> Incorrect use of \"del view\"\n");}
        return result;
    } else if(strcmp(argv[0], "save") == 0) {
        result = check_command(command, argv, argc, "save", COMMAND_SAVE_START_ARG, COMMAND_SAVE_ARG_SIZE);
        if (!result){
            printf("> Incorrect use of \"save\", there should be 1 argument (received %d) which is the aquarium to save.\n", argc - 1);
            write_in_log(check, "print", 0, 0, "> Incorrect use of \"save\"\n");}
        return result;
    }

    printf("> Unknown command : should be \"load\", \"save\", \"show\", \"add view\", \"del view\" or \"exit\" (received %s)\n", argv[0]);
    write_in_log(check, "unk", 0, 0, argv[0]);

    return 0;
}

void print_command(struct command * command){
    printf("============ Debug ============\n");
    printf("command name : %s\n", command->command_name);
    printf("number of arguments : %d\n", command->nb_params);
    for(int i=0 ; i < command->nb_params ; i++){
        printf("- argument %d : %s\n", i, command->params[i]); //bug valgrind
    }
    printf("===============================\n");
}


int check_add_wiew_format(char * input){
    size_t i = 0;
    int times_number = 0;
    int plus_number = 0;

    while (i < strlen(input)){
        if(input[i] == 'x') {         
            times_number += 1;

            if(plus_number != 0)
                return 0;
            
        } else if(input[i] == '+')    
            plus_number += 1;
        else {
            if(input[i] < '0' || input[i] > '9')
                return 0;
        }


        i++;
    }

    if(times_number == 1 && plus_number == 2)
        return 1;
        
    return 0;
}


void free_command(struct command * command){
    free(command->params);
    free(command);
}
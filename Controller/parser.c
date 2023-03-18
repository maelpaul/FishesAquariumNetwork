#include "parser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void parse_command(struct command * command, char** argv, int argc){
    if(strcmp(argv[1],"load") == 0){
        if(argc-2 != 1){
            printf("Uncorrect use of \"load\", there should be 1 argument (received %d) which is the aquarium to load.\n", argc-2);
        }
        else{
            command->command_name="load";
            command->nb_params = argc-2;
            int malloc_size = command->nb_params * sizeof(char*);
            command->params=malloc(malloc_size);
            command->params[0]=argv[2];
            command->_total_param_len = malloc_size;
        }
    }
    else if(strcmp(argv[1],"show") == 0){
        if(argc-2 != 1){
            printf("Uncorrect use of \"show\", there should be 1 argument (received %d) which is the aquarium to show.\n", argc-2);
        }
    }
    else if(strcmp(argv[1],"add") == 0 && strcmp(argv[2],"view") == 0){
        if(argc-3 != 2){
            printf("Uncorrect use of \"add view\", there should be 2 arguments (received %d) which are the name of the view and its size\n", argc-3);
        }
    }
    else if(strcmp(argv[1],"del") == 0){
        if(argc-3 != 1){
            printf("Uncorrect use of \"del view\", there should be 1 argument 1 (received %d) which is the name of the view to remove.\n", argc-3);
        }
    }
    else if(strcmp(argv[1],"save") == 0){
        if(argc-3 != 1){
            printf("Uncorrect use of \"save\", there should be 1 argument (received %d) which is the aquarium to save.\n", argc-2);
        }
    }
}

void free_command(struct command * command){
    free(command->params);
    free(command);
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

void init_command(struct command * command){
    command->command_name = NULL;
    command->nb_params = 0;
    command->params = NULL;
    command->_total_param_len = 0;
}

int main(int argc, char** argv){
    struct command * command = malloc(sizeof(struct command));
    
    init_command(command);

    if (argc < 3) {
        printf("failure\n");
        free_command(command);
        return EXIT_FAILURE;
    }

    // tested with ./parser load 54
    parse_command(command, argv, argc);
    
    //print
    print_command(command);
    
    //free
    free_command(command);
    
    return 0;
}
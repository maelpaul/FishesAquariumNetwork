#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

void _print_command(struct command * command){
    printf("command name : %s\n", command->command_name);
    printf("number of arguments : %d\n", command->nb_params);
    for(int i=0 ; i < command->nb_params ; i++){
        printf("- argument %d : %s\n", i, command->params[i]); //bug valgrind
    }
    printf("\n");
}

int main(int argc, char** argv){
    struct command * command = malloc(sizeof(struct command));
    
    init_command(command);

    // there should be at least 3 args  
    if (argc < 3) {
        printf("failure\n");
        free_command(command);
        return EXIT_FAILURE;
    }

    if(parse_command(command, argv, argc)){_print_command(command);}
    
    free_command(command);
    return EXIT_SUCCESS;
}
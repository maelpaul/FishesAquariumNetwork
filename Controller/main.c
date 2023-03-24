#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv){
    struct command * command = malloc(sizeof(struct command));
    
    init_command(command);

    // there should be at least 3 args  
    if (argc < 3) {
        printf("failure\n");
        free_command(command);
        return EXIT_FAILURE;
    }

    if(parse_command(command, argv, argc)){print_command(command);}
    
    free_command(command);
    return EXIT_SUCCESS;
}
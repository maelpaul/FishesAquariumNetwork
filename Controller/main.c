#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char** argv){
    struct command * command = malloc(sizeof(struct command));
    
    init_command(command);

    if (argc < 3) {
        printf("failure\n");
        free_command(command);
        return EXIT_FAILURE;
    }

    parse_command(command, argv, argc);
    
    //print
    print_command(command);
    
    //free
    free_command(command);
    return EXIT_SUCCESS;
}
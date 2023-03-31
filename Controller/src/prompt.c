#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"

#define BUFFER_SIZE 100

void clear(char * buffer) {
    int i = 0;
    while (buffer[i] != '\n' && buffer[i] != '\0') {
        buffer[i] = '\0';
        i++;
    }
    buffer[i] = '\0';
}

int length(char ** argv) {
    int i = 0;
    while (i < 4 && argv[i][0] != '\0') {
        i++;
    }
    return i;
}

int main()
{
    char buffer[BUFFER_SIZE];
    char arg1[BUFFER_SIZE];
    char arg2[BUFFER_SIZE];
    char arg3[BUFFER_SIZE];
    char arg4[BUFFER_SIZE];

    printf("enter your command :\n");
    
    while (1) {
        struct command * command = malloc(sizeof(struct command));
    
        init_command(command);

        clear(buffer);
        clear(arg1);
        clear(arg2);
        clear(arg3);
        clear(arg4);
        
        ssize_t a = read(0, buffer, BUFFER_SIZE);
        if (a == -1) {
            perror("read");
            return EXIT_FAILURE;
        }
        
        char * p = arg1;
        int i = 0;
        int j = 0;
        int c = 0;
        
        while (buffer[i] != '\n') {
            if (buffer[i] == ' ') {
                if (c == 0) {
                    p[j] = '\0';
                    i++;
                    j = 0;
                    p = arg2;
                    c++;
                }
                else if (c == 1) {
                    p[j] = '\0';
                    i++;
                    j = 0;
                    p = arg3;
                    c++;
                }
                else if (c == 2) {
                    p[j] = '\0';
                    i++;
                    j = 0;
                    p = arg4;
                    c++;
                }
                else {
                    return EXIT_FAILURE;
                }
            }
            p[j] = buffer[i];
            i++;
            j++;
        }
        p[j] = '\0';
        
        char * my_argv[4] = {arg1, arg2, arg3, arg4};
        int my_argc = length(my_argv); 
        if (my_argc < 2) {
            if (strcmp(arg1, "exit") == 0 || strcmp(arg1, "quit") == 0 || strcmp(arg1, "q") == 0) {
                return EXIT_SUCCESS;
            }
            printf("failure\n");
            free_command(command);
            return EXIT_FAILURE;
        }

        // for (int i = 0; i < my_argc; i++) {
        //     printf("argv[%d] = %s\n", i, my_argv[i]);
        // }

        if(parse_command(command, my_argv, my_argc)){print_command(command);printf("enter your command :\n");}
    
        free_command(command);
        
        if (strcmp(arg1, "exit") == 0 || strcmp(arg1, "quit") == 0 || strcmp(arg1, "q") == 0) {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int parse_command(char * arg1, char * arg2, char * arg3, char * arg4) {
    if(strcmp(arg1, "load") == 0) {
        return 1;
    }
    return 0;
}

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

int main(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];
    char arg1[BUFFER_SIZE];
    char arg2[BUFFER_SIZE];
    char arg3[BUFFER_SIZE];
    char arg4[BUFFER_SIZE];
    while (1) {
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
        for (int i = 0; i < my_argc; i++) {
            printf("argv[%d] = %s\n", i, my_argv[i]);
        }
        if (strcmp(arg1, "exit") == 0 || strcmp(arg1, "quit") == 0) {
            return EXIT_SUCCESS;
        }
        int r = parse_command(arg1, arg2, arg3, arg4);
        printf("%d\n", r);
    }
    return EXIT_SUCCESS;
}

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
#include "parser.h"

#define BUFFER_SIZE 100
#define MAX_ARG 4

void clear(char* str, int size) {
    memset(str, '\0', size);
}

void prompt()
{
    char buffer[BUFFER_SIZE];
    char args[MAX_ARG][BUFFER_SIZE];

    int client_fd;
    int portno = 12345;
    struct sockaddr_in serv_addr;
    int n;

    // Création du socket client
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Erreur lors de l'ouverture de la socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Adresse IP du serveur
    serv_addr.sin_port = htons(portno);

    // Connexion au serveur distant
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur lors de la connexion au serveur");
        exit(EXIT_FAILURE);
    }

    // Réception de la réponse du serveur
    clear(buffer, BUFFER_SIZE);
    if ((n = recv(client_fd, buffer, sizeof(buffer), 0)) < 0) {
        perror("Erreur lors de la réception de la réponse du serveur");
        exit(EXIT_FAILURE);
    }

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

        if((strcmp(args[0], "quit") == 0) || parse_command(command, my_argv, arg_count)){
            n = write(client_fd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Erreur lors de l'écriture sur la socket");
                exit(EXIT_FAILURE);
            }

            memset(buffer, 0, sizeof(buffer));
            if ((n = recv(client_fd, buffer, sizeof(buffer), 0)) < 0) {
                perror("Erreur lors de la réception de la réponse du serveur");
                exit(EXIT_FAILURE);
            }
            printf("Message du serveur : %s\n", buffer);
        }
        
        if (strcmp(buffer, "> Bye") == 0) {
            should_continue = 0;
        }
            
        free_command(command);
    }
}

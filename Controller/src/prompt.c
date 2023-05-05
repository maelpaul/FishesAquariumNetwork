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

#include "parser.h"

#define BUFFER_SIZE 100

void clear(char * buffer) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = '\0';
    }
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

    printf("Enter your command :\n");

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
    memset(buffer, 0, sizeof(buffer));
    if ((n = recv(client_fd, buffer, sizeof(buffer), 0)) < 0) {
        perror("Erreur lors de la réception de la réponse du serveur");
        exit(EXIT_FAILURE);
    }
    
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
            free_command(command);
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
                    free_command(command);
                    return EXIT_FAILURE;
                }
            }
            while (buffer[i] == ' ') {
                i++;
            }
            p[j] = buffer[i];
            i++;
            j++;
        }
        p[j] = '\0';
        
        char * my_argv[4] = {arg1, arg2, arg3, arg4};
        int my_argc = length(my_argv); 

        // for (int i = 0; i < my_argc; i++) {
        //     printf("argv[%d] = %s\n", i, my_argv[i]);
        // }

        if(parse_command(command, my_argv, my_argc)){
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
            printf(">\n%s\n", buffer);
        }
    
        free_command(command);
        
        if (strcmp(arg1, "exit") == 0 || strcmp(arg1, "quit") == 0 || strcmp(arg1, "q") == 0) {
            free_command(command);
            return EXIT_SUCCESS;
        }
    }
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "server_utils.h"

int main()
{
    struct config conf;
    char buffer[256];
    int n;

    load_config("controller.cfg", &conf);

    struct aquarium aquarium;

    load_initial_aquarium_config("aquarium_example.txt", &aquarium);

    // server and socket file descriptor
    int server_fd, newsockfd;
    //TCP port
    int portno = 12345;
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Erreur lors de l'ouverture de la socket'");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur lors de la liaison du socket");
        exit(EXIT_FAILURE);
    }

    // Met le socket en écoute sur le port spécifié
    if (listen(server_fd, 5) < 0) {
        perror("Erreur lors de la mise en écoute du socket");
        exit(EXIT_FAILURE);
    }

    // Accepte une connexion entrante
    if ((newsockfd = accept(server_fd, (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen)) < 0) {
        perror("Erreur lors de l'acceptation de la connexion entrante");
        exit(EXIT_FAILURE);
    }

    // Envoi d'un message au client
    strcpy(buffer, "Bonjour client !");
    if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }

    do {
        // Réception de la réponse du client
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(newsockfd, buffer, sizeof(buffer), 0)) < 0) {
            perror("Erreur lors de la réception de la réponse du client");
            exit(EXIT_FAILURE);
        }

        printf("Message du client : %s\n", buffer);

    } while(strcmp(buffer, "log out\n") != 0);

    // Envoi de sortie de connexion au client
    strcpy(buffer, "Bye");
    if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }

    close(server_fd);

    return 0;
} 
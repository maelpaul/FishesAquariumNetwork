#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client_fd;
    int portno = 12345;
    struct sockaddr_in serv_addr;
    char buffer[256];
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

    printf("Message du serveur : %s\n", buffer);

    do{
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            n = write(client_fd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Erreur lors de l'écriture sur la socket");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("Erreur lors de la lecture depuis l'entrée standard");
            exit(EXIT_FAILURE);
        }

    } while((strcmp(buffer, "log out\n") != 0));


    memset(buffer, 0, sizeof(buffer));
    if ((n = recv(client_fd, buffer, sizeof(buffer), 0)) < 0) {
        perror("Erreur lors de la réception de la réponse du serveur");
        exit(EXIT_FAILURE);
    }

    printf("Message du serveur : %s\n", buffer);

    // Fermeture de la connexion avec le serveur
    close(client_fd);

    return 0;
}

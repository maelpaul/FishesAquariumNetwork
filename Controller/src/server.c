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

    struct aquarium * aquarium = malloc(sizeof(struct aquarium));
    aquarium_init(aquarium);
    load_initial_aquarium_config("aquarium_example.txt", aquarium);
    aquarium_print(aquarium);
    aquarium_free(aquarium);
    
    // server and socket file descriptor
    int server_fd, newsockfd;
    int portno = conf.controller_port;
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

        // Initialisation et authentification
        if (!strncmp(buffer,"hello",5)) {
            char * view_name = "N1";
            char * attributed_view = client_connection(aquarium, view_name);
            // message au client
        }

        // Demande périodique des poissons
        char * ask_periodic_verif;
        strncpy (ask_periodic_verif , buffer, 10);
        ask_periodic_verif[9] = '\0';   /* null character manually added */

        if (!strcmp(ask_periodic_verif, "getFishes")) {
            // char fish_list[1024] = "list ";
            // for (int i = 0; i < aquarium->fishes_len; i++) {
            //     char fish_info[128];
            //     sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, aquarium->fishes[i]->dest[0], aquarium->fishes[i]->dest[1], aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->size[0]);
            //     strcat(fish_list, fish_info);
            // }
            // strcat(fish_list, "\n");
            char fish_list[1024] = "list ";
            if (send(newsockfd, fish_list, strlen(fish_list), 0) < 0) {
                perror("Erreur lors de l'envoi de la liste des poissons au client");
                exit(EXIT_FAILURE);
            }

        }

        // Demande continue de Poisson
        char * ask_continuous_verif;
        char * ls;
        strncpy (ask_continuous_verif , buffer, 21);
        strncpy (ls , buffer, 2);
        ask_continuous_verif[21] = '\0';   /* null character manually added */
        ls[2] = '\0';

        if (!strcmp(ask_continuous_verif, "getFishesContinuously") || !strcmp(ls, "ls")) {
            // Lister les poissons en continue
        }

        // Ajout d'un poisson
        char * add_verif;
        strncpy (add_verif, buffer, 7);
        add_verif[7] = '\0';   /* null character manually added */

        if (!strcmp(add_verif, "addFish")) {
            char * info;
            memcpy(info, buffer, 256);
            char delim[] = " ";

            /* Cut the buffer to separate the contents.*/
	        char * verif = strtok(info, delim);
            char * name = strtok(NULL, delim);
            char * at = strtok(NULL, delim);
            char * numbers = strtok(NULL, delim);
            char * path = strtok(NULL, delim);

            int * coords = atoi(strtok(numbers, ","));
            int * size = atoi(strtok(NULL, ","));
            
            if (!strcmp(path, "RandomWayPoint")) {
                void (*new_path)(struct fish *, int, int) = &RandomWayPoint;
                controller_add_fish(aquarium, coords, size, name, new_path);
            }

	        
        }

        // Suppression d'un poisson
        char * del_verif;
        strncpy (del_verif, buffer, 7);
        del_verif[7] = '\0';   /* null character manually added */

        if (!strcmp(del_verif, "delFish")) {
            // Suppression un poisson
        }

        // Démarrage d'un poisson
        char * start_verif;
        strncpy (start_verif, buffer, 9);
        start_verif[9] = '\0';   /* null character manually added */

        if (!strcmp(start_verif, "startFish")) {
            // Démarrage d'un poisson
        }

        // ping

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
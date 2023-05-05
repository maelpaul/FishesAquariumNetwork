#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#include "server_utils.h"

#define REFRESH_TIME 5

int main()
{
    srand(time(NULL));
    //time_t start_time = time(NULL);
    struct config conf;
    char buffer[256];
    int n;

    load_config("controller.cfg", &conf);

    struct aquarium * aquarium = malloc(sizeof(struct aquarium));
    aquarium_init(aquarium);
    
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

    const int enable = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
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

    int check = 0;
    int is_aquarium_loaded = 0;

    do{
        // command from prompt

        // reception
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(newsockfd, buffer, sizeof(buffer), 0)) < 0) {
            perror("Erreur lors de la réception de la réponse du client");
            exit(EXIT_FAILURE);
        }

        char load_verif[5];
        strncpy (load_verif, buffer, 4);
        load_verif[4] = '\0';
        if (!strcmp(load_verif, "load")) {
            check = 1;
            strcpy(buffer, "aquarium loaded");
            load_initial_aquarium_config("aquarium_example.txt", aquarium);
            aquarium_print(aquarium);
            is_aquarium_loaded=1;
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }

    }while(is_aquarium_loaded == 0);

    do {

        // time_t new_time = time(NULL);
        // if (difftime(new_time, start_time) >= REFRESH_TIME) {
        //     start_time = new_time;
        //     for (int i = 0; i < aquarium->fishes_len; i++) {
        //         aquarium->fishes[i]->path(aquarium->fishes[i], aquarium->size[0], aquarium->size[1]);
        //     }
        // }

        time_t current_time = time(NULL);
        controller_update_fishes(aquarium, current_time, REFRESH_TIME);

        // Réception de la réponse du client
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(newsockfd, buffer, sizeof(buffer), 0)) < 0) {
            perror("Erreur lors de la réception de la réponse du client");
            exit(EXIT_FAILURE);
        }

        printf("Message du client : %s\n", buffer);
                // command from prompt
        char load_verif[5];
        strncpy (load_verif, buffer, 4);
        load_verif[4] = '\0';
        if (!strcmp(load_verif, "load")) {
            check = 1;
            strcpy(buffer, "oui");
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        // Initialisation et authentification
        if (!strncmp(buffer,"hello",5)) {
            check = 1;
            char * view_name = NULL;
            int x = 1;
            if(strlen(buffer)!=6){
                x = hello_command_check(buffer, view_name);
            }
            if(x){
                char * attributed_view = client_connection(aquarium, view_name);
                if(strcmp(attributed_view,"no greeting")==0){
                    strcpy(buffer, attributed_view);
                    if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                        perror("Erreur lors de l'envoi du message au client");
                        exit(EXIT_FAILURE);
                    }
                }
                else{
                    char to_send[64] = "greeting ";
                    strcat(to_send, attributed_view);
                    strcpy(buffer, to_send);
                    if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                        perror("Erreur lors de l'envoi du message au client");
                        exit(EXIT_FAILURE);
                    } 
                }
            }
            else{
                char to_send[64] = "incorrect format";
                strcpy(buffer, to_send);
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                } 
            }
        }

        // Demande continue de Poisson
        int continuous = 0;
        char ask_continuous_verif[22];
        char ls[3];
        strncpy (ask_continuous_verif , buffer, 21);
        strncpy (ls , buffer, 2);
        ask_continuous_verif[21] = '\0';   /* null character manually added */
        ls[2] = '\0';

        if (!strcmp(ask_continuous_verif, "getFishesContinuously") || !strcmp(ls, "ls")) {
            continuous = 1;
            check = 1;
            // Lister les poissons en continue
            for (int i = 0; i < 10; ++i) {
                char fish_list[1024] = "list ";
                for (int i = 0; i < aquarium->fishes_len; i++) {
                    char fish_info[128];
                    sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, aquarium->fishes[i]->dest[0], aquarium->fishes[i]->dest[1], aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->time_to_dest);
                    strcat(fish_list, fish_info);
                }
                strcat(fish_list, "\n");
                if (send(newsockfd, fish_list, strlen(fish_list), 0) < 0) {
                    perror("Erreur lors de l'envoi de la liste des poissons au client");
                    exit(EXIT_FAILURE);
                }
                if (i != 9) {
                    sleep(1);
                    current_time = time(NULL);
                    controller_update_fishes(aquarium, current_time, REFRESH_TIME);
                }                
            }
        }

        // Demande périodique des poissons
        if (continuous == 0) {
            char ask_periodic_verif[10];
            strncpy (ask_periodic_verif , buffer, 10);
            ask_periodic_verif[9] = '\0';   /* null character manually added */

            if (!strcmp(ask_periodic_verif, "getFishes")) {
                check = 1;
                char fish_list[1024] = "list ";
                for (int i = 0; i < aquarium->fishes_len; i++) {
                    char fish_info[128];
                    sprintf(fish_info, "> [%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, aquarium->fishes[i]->dest[0], aquarium->fishes[i]->dest[1], aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->time_to_dest);
                    strcat(fish_list, fish_info);
                }
                strcat(fish_list, "\n");
                if (send(newsockfd, fish_list, strlen(fish_list), 0) < 0) {
                    perror("Erreur lors de l'envoi de la liste des poissons au client");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // Ajout d'un poisson
        char add_verif[8];
        strncpy (add_verif, buffer, 7);
        add_verif[7] = '\0';   /* null character manually added */

        if (!strcmp(add_verif, "addFish")) {
            check = 1;

            char info[256];
            memcpy(info, buffer, 256);
            char delim[] = " ";

            /* Cut the buffer to separate the contents.*/
	        char * verif = strtok(info, delim);
            (void) verif;
            char * name = strtok(NULL, delim);
            char * at = strtok(NULL, delim);
            (void) at;
            char * __coords = strtok(NULL, delim);
            char * __size = strtok(NULL, delim);
            char * _path = strtok(NULL, delim);
            char * path = strtok(_path, "\n");
            
            char * _coords = strtok(__coords, ",");
            char * _size = strtok(__size, ",");

            int coords[2] = {-1, -1};
            char * a_c = strtok(_coords, "x");
            char * b_c = strtok(NULL, "x");
            if (a_c != NULL && b_c != NULL) {
                coords[0] = atoi(a_c);
                coords[1] = atoi(b_c);
            }

            int size[2] = {-1, -1};
            char * a_s = strtok(_size, "x");
            char * b_s = strtok(NULL, "x");
            if (a_s != NULL && b_s != NULL) {
                size[0] = atoi(a_s);
                size[1] = atoi(b_s);
            }

            int val = -1;
            int check_path = 0;
            
            if (path != NULL) {
                if (!strcmp(path, "RandomWayPoint")) {
                    check_path = 1;
                    void (*new_path)(struct fish *, int, int) = &RandomWayPoint;
                    val = controller_add_fish(aquarium, coords, size, name, new_path);
                }
            }

            if (check_path == 0 && path != NULL) {
                strcpy(buffer, "> NOK : Modèle de mobilité non supporté");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else if (val == 0 && check_path == 1) {
                strcpy(buffer, "> NOK : Poisson déjà existant");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else if (val == 1 && check_path == 1) {
                strcpy(buffer, "> OK");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                } 
            }
            else {
                strcpy(buffer, "> NOK : Syntaxe invalide");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }                 
            } 
        }

        // Suppression d'un poisson
        char del_verif[8];
        strncpy (del_verif, buffer, 7);
        del_verif[7] = '\0';   /* null character manually added */

        if (!strcmp(del_verif, "delFish")) {
            check = 1;
            // Suppression un poisson
            char info[256];
            memcpy(info, buffer, 256);
            char delim[] = " ";

            char * verif = strtok(info, delim);
            (void) verif;
            char * _fish = strtok(NULL, delim);
            char * fish = strtok(_fish, "\n");

            int val = controller_del_fish(aquarium, fish);
            if (val == 0) {
                strcpy(buffer, "> NOK : Poisson inexistant");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else if (val == 1) {
                strcpy(buffer, "> OK");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                } 
            }
            else {
                strcpy(buffer, "> NOK : Nom de poisson invalide");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }                 
            }
        }

        // Démarrage d'un poisson
        char start_verif[10];
        strncpy (start_verif, buffer, 9);
        start_verif[9] = '\0';   /* null character manually added */

        if (!strcmp(start_verif, "startFish")) {
            check = 1;
            // Démarrage d'un poisson
            char info[256];
            memcpy(info, buffer, 256);
            char delim[] = " ";

            /* Cut the buffer to separate the contents.*/
	        char * verif = strtok(info, delim);
            (void) verif;
            char * _name = strtok(NULL, delim);
            char * name = strtok(_name, "\n");

            int val = controller_start_fish(aquarium, name, current_time, REFRESH_TIME);
            if (val == 0) {
                strcpy(buffer, "NOK : Poisson inexistant");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else if (val == 1) {
                strcpy(buffer, "OK");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                } 
            }
            else {
                strcpy(buffer, "NOK : Nom de poisson invalide");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }                 
            }
        }

        // ping
        char ping_verif[5];
        strncpy (ping_verif, buffer, 4);
        ping_verif[4] = '\0';   /* null character manually added */

        if (!strcmp(ping_verif, "ping")) {
            check = 1;
            strcpy(buffer, "pong");
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }

        // check des commandes inexistantes
        if (check == 0 && strcmp(buffer, "log out\n") != 0) {
            strcpy(buffer, "Commande inexistante");
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }
    } while(strcmp(buffer, "log out\n") != 0);

    // Envoi de sortie de connexion au client
    strcpy(buffer, "Bye");
    if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }

    aquarium_free(aquarium);
    close(server_fd);

    return 0;
} 
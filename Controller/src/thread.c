#include "thread.h"

int client(void * client_arg)
{

    struct client_arg * args = (struct client_arg *) client_arg;
    char buffer[256];
    int n;

    int newsockfd = args->newsockfd;
    struct aquarium * aquarium = args->aquarium;

    do {
        int check = 0;

        // Réception de la réponse du client
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(newsockfd, buffer, sizeof(buffer), 0)) < 0) {
            perror("Erreur lors de la réception de la réponse du client");
            exit(EXIT_FAILURE);
        }
        int test = 0;
        for(long unsigned int i=0;i<5;i++){
            if (buffer[test]=='\0'){
                test++;
            }
        }
        if (test<=2) {
            printf("Message du client : %s\n", buffer);
        }

        // command from prompt
        char load_verif[5];
        strncpy (load_verif, buffer, 4);
        load_verif[4] = '\0';
        if (!strcmp(load_verif, "load")) {
            check = 1;
            
            char info[256];
            memcpy(info, buffer, 256);
            char delim[] = " ";

            char * verif = strtok(info, delim);
            (void) verif;
            char * _aquarium_name = strtok(NULL, delim);
            char * aquarium_name = strtok(_aquarium_name, "\n");

            if (!strcmp(aquarium_name, "aquarium")) {
                strcpy(buffer, "> OK : aquarium already loaded");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else {
                strcpy(buffer, "> NOK : aquarium not existing");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
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
                    char to_send[64] = "> ";
                    strcat(to_send, attributed_view);
                    strcpy(buffer, to_send);
                    if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                        perror("Erreur lors de l'envoi du message au client");
                        exit(EXIT_FAILURE);
                    }
                }
                else{
                    char to_send[64] = "> greeting ";
                    strcat(to_send, attributed_view);
                    strcpy(buffer, to_send);
                    if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                        perror("Erreur lors de l'envoi du message au client");
                        exit(EXIT_FAILURE);
                    } 
                }
            }
            else{
                char to_send[64] = "> incorrect format";
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
                controller_update_fishes(aquarium, REFRESH_TIME);
                char fish_list[1024] = "> list ";
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
                controller_update_fishes(aquarium, REFRESH_TIME);
                char fish_list[1024] = "> list ";
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

            int val = controller_start_fish(aquarium, name, REFRESH_TIME);
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
            else if (val == 2) {
                strcpy(buffer, "> NOK : Poisson déjà démaré");
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

        // statut
        char status_verif[7];
        strncpy (status_verif, buffer, 6);
        status_verif[6] = '\0';   /* null character manually added */

        if (!strcmp(status_verif, "status")) {
            check = 1;
            controller_update_fishes(aquarium, REFRESH_TIME);
            // Affichage statut
            int nb_fishes = aquarium->fishes_len;
            char len[10];
            sprintf(len, "%d ", nb_fishes);
            char info[1024] = "> OK : Connecté au contrôleur, ";
            strcat(info, len);

            if (nb_fishes == 0 || nb_fishes == 1) {
                char info2[100] = "poisson trouvé";
                strcat(info, info2);
            }
            else {
                char info2[100] = "poissons trouvés";
                strcat(info, info2);
            }
            strcat(info, "\n");

            for (int i = 0; i < aquarium->fishes_len; i++) {
                char fish_info[128];
                sprintf(fish_info, "Fish %s at %dx%d, %dx%d ", aquarium->fishes[i]->name, aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->size[0], aquarium->fishes[i]->size[1]);
                if (aquarium->fishes[i]->started == 1) {
                    strcat(fish_info, "started");
                }
                else {
                    strcat(fish_info, "notStarted");
                }
                strcat(fish_info, "\n");
                strcat(info, fish_info);
            }
            if (send(newsockfd, info, strlen(info), 0) < 0) {
                perror("Erreur lors de l'envoi de la liste des poissons au client");
                exit(EXIT_FAILURE);
            }
        }

        // ping
        char ping_verif[5];
        strncpy (ping_verif, buffer, 4);
        ping_verif[4] = '\0';   /* null character manually added */

        if (!strcmp(ping_verif, "ping")) {
            check = 1;

            char info[256];
            memcpy(info, buffer, 256);
            char delim[] = " ";

            char * verif = strtok(info, delim);
            (void) verif;
            char * _channel = strtok(NULL, delim);
            char * channel = strtok(_channel, "\n");

            if (channel == NULL) {
                strcpy(buffer, "> NOK : numéro de port invalide");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else if (!strcmp(channel, "12345")) {
                strcpy(buffer, "> pong 12345");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else {
                strcpy(buffer, "> NOK : mauvais numéro de port");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // show command from prompt
        char show_verif[5];
        strncpy (show_verif, buffer, 4);
        show_verif[4] = '\0';
        if (!strcmp(show_verif, "show")) {
            check = 1;

            char info[256];
            memcpy(info, buffer, 256);
            char delim[] = " ";

            char * verif = strtok(info, delim);
            (void) verif;
            char * _aquarium_name = strtok(NULL, delim);
            char * aquarium_name = strtok(_aquarium_name, "\n");

            if (!strcmp(aquarium_name, "aquarium")) {
                char to_send[256] = "> OK : \n";
                controller_aquarium_print(aquarium, to_send);
                strcpy(buffer, to_send);
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else {
                strcpy(buffer, "> NOK : aquarium not existing");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // check des commandes inexistantes
        if (check == 0 && strcmp(buffer, "log out\n") != 0) {
            strcpy(buffer, "> NOK : Commande inexistante");
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }
    } while(strcmp(buffer, "log out\n") != 0);

    return 0;
}

int client_bidon(int newsockfd, struct aquarium * aquarium)
{
    return 0;
}
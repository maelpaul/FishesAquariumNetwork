#include "command_getfish.h"

int get_fish_server(char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id){
    char ask_periodic_verif[10];
    strncpy (ask_periodic_verif , buffer, 10);
    ask_periodic_verif[9] = '\0';   /* null character manually added */

    if (!strcmp(ask_periodic_verif, "getFishes")) {
        pthread_mutex_lock(mutex);
        controller_update_fishes(aquarium, REFRESH_TIME);
        pthread_mutex_unlock(mutex);
        char fish_list[1024] = "> list ";
        for (int i = 0; i < aquarium->fishes_len; i++) {
            char fish_info[128];
            sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, aquarium->fishes[i]->dest[0], aquarium->fishes[i]->dest[1], aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->time_to_dest);
            strcat(fish_list, fish_info);
        }
        strcat(fish_list, "\n");
        if (send(client_id, fish_list, strlen(fish_list), 0) < 0) {
            perror("Erreur lors de l'envoi de la liste des poissons au client");
            exit(EXIT_FAILURE);
        }
        return 1;
    }
    return 0;
}

int get_fish_continuously_server(char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id){
    char ask_continuous_verif[22];
    char ls[3];
    strncpy (ask_continuous_verif , buffer, 21);
    strncpy (ls , buffer, 2);
    ask_continuous_verif[21] = '\0';   /* null character manually added */
    ls[2] = '\0';

    if (!strcmp(ask_continuous_verif, "getFishesContinuously") || !strcmp(ls, "ls")) {
        // Lister les poissons en continu
        for (int i = 0; i < 10; ++i) {
            pthread_mutex_lock(mutex);
            controller_update_fishes(aquarium, REFRESH_TIME);
            pthread_mutex_unlock(mutex);
            char fish_list[1024] = "> list ";
            for (int i = 0; i < aquarium->fishes_len; i++) {
                char fish_info[128];
                sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, aquarium->fishes[i]->dest[0], aquarium->fishes[i]->dest[1], aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->time_to_dest);
                strcat(fish_list, fish_info);
            }
            strcat(fish_list, "\n");
            if (send(client_id, fish_list, strlen(fish_list), 0) < 0) {
                perror("Erreur lors de l'envoi de la liste des poissons au client");
                exit(EXIT_FAILURE);
            }
            if (i != 9) {
                sleep(1);
            }                
        }
        return 1;
    }
    return 0;
}

int get_status_server(char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id){
    char status_verif[7];
    strncpy (status_verif, buffer, 6);
    status_verif[6] = '\0';   /* null character manually added */

    if (!strcmp(status_verif, "status")) {
        pthread_mutex_lock(mutex);
        controller_update_fishes(aquarium, REFRESH_TIME);
        pthread_mutex_unlock(mutex);
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
        if (send(client_id, info, strlen(info), 0) < 0) {
            perror("Erreur lors de l'envoi de la liste des poissons au client");
            exit(EXIT_FAILURE);
        }
        return 1;
    }
    return 0;
}

int ping_server(char * buffer, int client_id) {
    char ping_verif[5];
    strncpy (ping_verif, buffer, 4);
    ping_verif[4] = '\0';   /* null character manually added */

    if (!strcmp(ping_verif, "ping")) {
        char info[256];
        memcpy(info, buffer, 256);
        char delim[] = " ";

        char * verif = strtok(info, delim);
        (void) verif;
        char * _channel = strtok(NULL, delim);
        char * channel = strtok(_channel, "\n");

        if (channel == NULL) {
            strcpy(buffer, "> NOK : numéro de port invalide");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        else if (!strcmp(channel, "12345")) {
            strcpy(buffer, "> pong 12345");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        else {
            strcpy(buffer, "> NOK : mauvais numéro de port");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        return 1;
    }
    return 0;
}

#include "command_getfish.h"

int get_fish_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, struct view * client_view){
    char ask_periodic_verif[10];
    strncpy (ask_periodic_verif , buffer, 10);
    ask_periodic_verif[9] = '\0';   /* null character manually added */

    if (!strcmp(ask_periodic_verif, "getFishes")) {
        pthread_mutex_lock(mutex);
        controller_update_fishes(aquarium, REFRESH_TIME);
        char fish_list[1024];
        strcpy(fish_list, header);
        strcat(fish_list, "|list ");
        for (int i = 0; i < aquarium->fishes_len; i++) {
            char fish_info[128];
            int fish_dest[2];
            fish_dest[0] = ((aquarium->fishes[i]->dest[0] - client_view->coords[0]) * 100) / client_view->size[0];
            fish_dest[1] = ((aquarium->fishes[i]->dest[1] - client_view->coords[1]) * 100) / client_view->size[1];
            int fish_size[2];
            fish_size[0] = (aquarium->fishes[i]->size[0] * 100) / client_view->size[0];
            fish_size[1] = (aquarium->fishes[i]->size[1] * 100) / client_view->size[1];
            sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, fish_dest[0], fish_dest[1], fish_size[0], fish_size[1], (int) ((aquarium->fishes[i]->time_to_dest + 10000) / 1000000));
            strcat(fish_list, fish_info);
        }
        strcat(fish_list, "\n");
        if (send(client_id, fish_list, strlen(fish_list), 0) < 0) {
            perror("Erreur lors de l'envoi de la liste des poissons au client");
            exit(EXIT_FAILURE);
        }
        write_in_log(check_ls, "send", 0, client_number, fish_list);
        pthread_mutex_unlock(mutex);
        return 1;
    }
    
    return 0;   
}

int get_fish_ls_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, struct view * client_view){
    char ls[3];
    strncpy (ls , buffer, 2);
    ls[2] = '\0';

    if (!strcmp(ls, "ls")) {
        // Lister les poissons en continu
        for (int j = 0; j < 10; ++j) {
            pthread_mutex_lock(mutex);
            controller_update_fishes(aquarium, REFRESH_TIME);
            char fish_list[1024];
            strcpy(fish_list, header);
            strcat(fish_list, "|list ");
            for (int i = 0; i < aquarium->fishes_len; i++) {
                char fish_info[128];
                int fish_dest[2];
                fish_dest[0] = ((aquarium->fishes[i]->dest[0] - client_view->coords[0]) * 100) / client_view->size[0];
                fish_dest[1] = ((aquarium->fishes[i]->dest[1] - client_view->coords[1]) * 100) / client_view->size[1];
                int fish_size[2];
                fish_size[0] = (aquarium->fishes[i]->size[0] * 100) / client_view->size[0];
                fish_size[1] = (aquarium->fishes[i]->size[1] * 100) / client_view->size[1];
                sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, fish_dest[0], fish_dest[1], fish_size[0], fish_size[1], (int) ((aquarium->fishes[i]->time_to_dest + 10000) / 1000000));
                strcat(fish_list, fish_info);
            }
            strcat(fish_list, "\n");
            if (send(client_id, fish_list, strlen(fish_list), 0) < 0) {
                perror("Erreur lors de l'envoi de la liste des poissons au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(check_ls, "send", 0, client_number, fish_list);
            pthread_mutex_unlock(mutex);
            if (j != 9) {
                sleep(1);
            }              
        }
        return 1;
    }
    return 0;
}

void * wait_client_log_out(void * arg){
        // Lister les poissons en continu
        struct for_thread * th = (struct for_thread *) arg;
        struct aquarium * aquarium = th->aquarium;
        struct view * client_view = th->client_view;
        while(1) {
            pthread_mutex_lock(th->mutex);
            controller_update_fishes(aquarium, REFRESH_TIME);
            char fish_list[1024];
            strcpy(fish_list, th->header);
            strcat(fish_list, "|list ");
            for (int i = 0; i < aquarium->fishes_len; i++) {
                char fish_info[128];
                int fish_dest[2];
                fish_dest[0] = ((aquarium->fishes[i]->dest[0] - client_view->coords[0]) * 100) / client_view->size[0];
                fish_dest[1] = ((aquarium->fishes[i]->dest[1] - client_view->coords[1]) * 100) / client_view->size[1];
                int fish_size[2];
                fish_size[0] = (aquarium->fishes[i]->size[0] * 100) / client_view->size[0];
                fish_size[1] = (aquarium->fishes[i]->size[1] * 100) / client_view->size[1];
                sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, fish_dest[0], fish_dest[1], fish_size[0], fish_size[1], (int) ((aquarium->fishes[i]->time_to_dest + 10000) / 1000000));
                strcat(fish_list, fish_info);
            }
            strcat(fish_list, "\n");
            
            if (send(th->client_id, fish_list, strlen(fish_list), 0) < 0) {
                perror("Erreur lors de l'envoi de la liste des poissons au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(th->check_ls, "send", 0, th->client_number, fish_list);

            pthread_mutex_unlock(th->mutex);
            sleep(1);  
        }
}

int get_fish_continuously_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, int buffer_size, struct view * client_view){
    pthread_t wait_log_out;
    struct for_thread th = {mutex, aquarium, header, client_id, check_ls, client_number, client_view};

    char verif[22];
    strncpy (verif , buffer, 21);
    verif[21] = '\0';
    int n;

    if (!strcmp(verif, "getFishesContinuously") && !strcmp(header, "gfc")) {
        pthread_create(&wait_log_out, NULL, wait_client_log_out, (void *) &th);
            
        // Réception de la réponse du client
        memset(buffer, 0, buffer_size);
        if ((n = recv(client_id, buffer, sizeof(buffer), 0)) < 0) {
            perror("Erreur lors de la réception de la réponse du client");
            exit(EXIT_FAILURE);
        }
        else if (n == 0) {
            pthread_cancel(wait_log_out);
            pthread_join(wait_log_out, NULL);
            return 1;
        }
    }
    else if (!strcmp(verif, "getFishesContinuously")) {
        return 2;
    }
    return 0;
}

int get_status_server(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, struct view * client_view){
    char status_verif[7];
    strncpy (status_verif, buffer, 6);
    status_verif[6] = '\0';   /* null character manually added */

    if (!strcmp(status_verif, "status")) {
        pthread_mutex_lock(mutex);
        controller_update_fishes(aquarium, REFRESH_TIME);
        // Affichage statut
        int nb_fishes = aquarium->fishes_len;
        char len[10];
        sprintf(len, "%d ", nb_fishes);
        char info[1024]; 
        strcpy(info, header);
        strcat(info, "|OK : Connecté au contrôleur, ");
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
            int fish_coords[2];
            fish_coords[0] = ((aquarium->fishes[i]->coords[0] - client_view->coords[0]) * 100) / client_view->size[0];
            fish_coords[1] = ((aquarium->fishes[i]->coords[1] - client_view->coords[1]) * 100) / client_view->size[1];
            int fish_size[2];
            fish_size[0] = (aquarium->fishes[i]->size[0] * 100) / client_view->size[0];
            fish_size[1] = (aquarium->fishes[i]->size[1] * 100) / client_view->size[1];
            sprintf(fish_info, "Fish %s at %dx%d, %dx%d ", aquarium->fishes[i]->name, fish_coords[0], fish_coords[1], fish_size[0], fish_size[1]);
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
        write_in_log(check_ls, "send", 0, client_number, info);
        pthread_mutex_unlock(mutex);
        return 1;
    }
    return 0;
}

int ping_server(int check_ls, int client_number, char * header, char * buffer, int client_id) {
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
            strcpy(buffer, header);
            strcat(buffer, "|NOK : numéro de port invalide");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(check_ls, "send", 0, client_number, buffer);
        }
        else if (!strcmp(channel, "12345")) {
            strcpy(buffer, header);
            strcat(buffer, "|pong 12345");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(check_ls, "send", 0, client_number, buffer);
        }
        else {
            strcpy(buffer, header);
            strcat(buffer, "|NOK : mauvais numéro de port");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(check_ls, "send", 0, client_number, buffer);
        }
        return 1;
    }
    return 0;
}

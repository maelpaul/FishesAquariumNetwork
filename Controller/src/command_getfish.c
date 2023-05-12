#include "command_getfish.h"

int get_fish_server(char * buffer, struct aquarium * aquarium, int client_id){
    
    char ask_periodic_verif[10];
    strncpy (ask_periodic_verif , buffer, 10);
    ask_periodic_verif[9] = '\0';   /* null character manually added */

    if (!strcmp(ask_periodic_verif, "getFishes")) {
        controller_update_fishes(aquarium, REFRESH_TIME);
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

int get_fish_continuously_server(char * buffer, struct aquarium * aquarium, int client_id){
    char ask_continuous_verif[22];
    char ls[3];
    strncpy (ask_continuous_verif , buffer, 21);
    strncpy (ls , buffer, 2);
    ask_continuous_verif[21] = '\0';   /* null character manually added */
    ls[2] = '\0';

    if (!strcmp(ask_continuous_verif, "getFishesContinuously") || !strcmp(ls, "ls")) {
        // Lister les poissons en continu
        for (int i = 0; i < 10; ++i) {
            controller_update_fishes(aquarium, REFRESH_TIME);
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
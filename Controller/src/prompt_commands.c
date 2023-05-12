#include "prompt_commands.h"

void prompt_load(char * buffer, struct aquarium * aquarium, int client_id, int * aquarium_flag, pthread_mutex_t * mutex_aquarium){
    char load_verif[5];
    strncpy (load_verif, buffer, 4);
    load_verif[4] = '\0';

    if (!strcmp(load_verif, "load")) {
        char info[256];
        memcpy(info, buffer, 256);
        char delim[] = " ";

        char * verif = strtok(info, delim);
        (void) verif;
        char * _aquarium_name = strtok(NULL, delim);
        char * aquarium_name = strtok(_aquarium_name, "\n");

        if (!strcmp(aquarium_name, "aquarium")) {
            strcpy(buffer, "> OK : aquarium loaded (");
            pthread_mutex_lock(mutex_aquarium);
            load_initial_aquarium_config("aquarium_example.txt", aquarium);
            pthread_mutex_unlock(mutex_aquarium);
            int nb_views = aquarium->views_len;
            char len[10];
            sprintf(len, "%d ", nb_views);
            strcat(buffer, len);

            if (nb_views == 0 || nb_views == 1) {
                strcat(buffer, "display view)!");
            }
            else {
                strcat(buffer, "display views)!");    
            }

            *aquarium_flag=1;

            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        else {
            strcpy(buffer, "> NOK : aquarium not existing");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int prompt_add_view(char * buffer, struct aquarium * aquarium, int client_id){
    char add_view_verif[9];
    strncpy (add_view_verif, buffer, 9);
    add_view_verif[8] = '\0';
    if (!strcmp(add_view_verif, "add view")) {
        char info[256];
        
        memcpy(info, buffer, 256);
        char delim[] = " ";

        char * view_name = strtok(info, delim);
        view_name = strtok(NULL, delim);
        view_name = strtok(NULL, delim);
        int coords[2];
        coords[0]=atoi(strtok(NULL,"x"));
        coords[1]=atoi(strtok(NULL,"+"));
        int size[2];
        size[0]=atoi(strtok(NULL,"+"));
        size[1]=atoi(strtok(NULL,"\n"));

        if(view_name_check(aquarium, view_name)){
            strcpy(buffer, "> NOK : View already exists");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
            return 0;
        }
        else{
            add_view(aquarium, coords, size, view_name);
            char to_send[256] = "> OK : view ";
            strcat(to_send, view_name);
            strcat(to_send, " added.");
            strcpy(buffer, to_send);
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            return 1;
        }
    }
    return 0;
}

int prompt_del_view(char * buffer, struct aquarium * aquarium, int client_id){
    char del_view_verif[9];
    strncpy (del_view_verif, buffer, 9);
    del_view_verif[8] = '\0';
    if (!strcmp(del_view_verif, "del view")) {
        char info[256];
        memcpy(info, buffer, 256);
        char delim[] = " ";

        char * view_name = strtok(info, delim);
        view_name = strtok(NULL, delim);
        view_name = strtok(NULL, "\n");
        
        int removed = 0;
        for(int i=0; i<aquarium->views_len; i++){
            if(strcmp(aquarium->views[i]->name,view_name)==0){
                del_view(aquarium, view_name);
                removed = 1;
                char to_send[256] = "> OK : view ";
                strcat(to_send, view_name);
                strcat(to_send, " deleted.");
                strcpy(buffer, to_send);
                if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
                return 1;
            }
        }
        if(!removed){
            strcpy(buffer, "> NOK : view not existing");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            return 0;
        }
    }
    return 0;
}

int prompt_save(char * buffer, struct aquarium * aquarium, int client_id){
    char show_verif[5];
    strncpy (show_verif, buffer, 4);
    show_verif[4] = '\0';
    if (!strcmp(show_verif, "show")) {
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
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            return 1;
        }
        else {
            strcpy(buffer, "> NOK : aquarium not existing");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            return 0;
        }
    }
    return 0;
}

int prompt_show(char * buffer, struct aquarium * aquarium, int client_id){    
    char save_verif[5];
    strncpy (save_verif, buffer, 5);
    save_verif[4] = '\0';
    if (!strcmp(save_verif, "save")) {
        char info[256];
        
        memcpy(info, buffer, 256);
        char delim[] = " ";

        char * aquarium_name = strtok(info, delim);
        aquarium_name = strtok(NULL, "\n");

        if (!strcmp(aquarium_name, "aquarium")) {
            aquarium = aquarium; // avoid unused paramter error
            //save_aquarium(aquarium); segfault
            strcpy(buffer, "> OK : aquarium saved");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            return 1;
        }
        else {
            strcpy(buffer, "> NOK : aquarium not existing");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            return 0;
        }
    }
    return 0;
}
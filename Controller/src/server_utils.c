#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "server_utils.h"

int load_config(const char *filename, struct config *conf)
{
    FILE *fp;
    char line[256], key[256], value[256];

    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open %s\n", filename);
        return -1;
    }

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%255s = %255s", key, value) == 2) {
            if (strcmp(key, "controller-port") == 0) {
                conf->controller_port = atoi(value);
            } else if (strcmp(key, "display-timeout-value") == 0) {
                conf->display_timeout_value = atoi(value);
            } else if (strcmp(key, "fish-update-interval") == 0) {
                conf->fish_update_interval = atoi(value);
            }
        }
    }

    fclose(fp);

    return 0;
}

int load_initial_aquarium_config(const char *filename, struct aquarium *aquarium, const char * aquarium_name){
    FILE *fp;
    char line[256], buf1[256], buf2[256];

    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open %s\n", filename);
        return -1;
    }

    int size[2] = {0,0};
    fgets(line, sizeof(line), fp);
    sscanf(line, "%255s", buf1);
    size[0] = atoi(strtok(buf1,"x"));
    size[1] = atoi(strtok(NULL,"x"));
    aquarium_create(aquarium, size, aquarium_name);

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%255s %255s", buf1, buf2) == 2) {
            int tokens[4];
            char * token = strtok(buf2, "x+");
            int i=0;

            while (token != NULL) {
                tokens[i] = atoi(token);
                i++;
                token = strtok(NULL, "x+");
            }
            int coords[2] = {tokens[0],tokens[1]};
            int size[2] = {tokens[2],tokens[3]};
            add_view(aquarium, coords, size, buf1);
        }   
    }

    fclose(fp);
    
    return 0;
}

void save_aquarium(struct aquarium * aquarium, const char * aquarium_name){
    FILE *fp;
    char filename[256];
    strcpy(filename, "../Controller/aquariums/");
    strcat(filename,aquarium_name);
    strcat(filename,".txt");
    fp = fopen(filename, "w+");

    if (fp == NULL) {
        printf("Failed to create file\n");
        return;
    }

    char to_send[1024] = "";
    controller_aquarium_print(aquarium, to_send);
    fprintf(fp, "%s", to_send);

    fclose(fp);
}

int controller_add_fish(struct aquarium *aquarium, int * coords, int * size, char * name, void (*path)(struct fish *, int, int)){
    int check = 1;
    for (int i = 0; i < 2; i++) {
        if (coords[i] == -1 || size[i] == -1) {
            check = 0;
        }
    }
    if (name != NULL && path != NULL && check != 0) {
        int val = add_fish(aquarium, coords, size, name, path);
        return val;
    }
    return -1;
}

int controller_del_fish(struct aquarium *aquarium, char *fish){
    if (fish != NULL) {
        int val = del_fish(aquarium, fish);
        return val;
    }
    return -1;
}

char * client_connection(struct aquarium *aquarium, char* view_name, struct view * client_view){
    if(view_name == NULL || !view_name_check(aquarium, view_name)){
        return find_and_attibute_free_view(aquarium, client_view);
    }
    else{
        for(int i=0 ; i < aquarium->views_len ; i++){
            if(strcmp(aquarium->views[i]->name,view_name) == 0){
                if(is_view_free(aquarium->views[i])){
                    change_view_status(aquarium->views[i]);
                    change_view_status(client_view);
                    strcpy(client_view->name, aquarium->views[i]->name);
                    return aquarium->views[i]->name;   
                }
                else{
                    return find_and_attibute_free_view(aquarium, client_view);
                }              
            }
        }
    }
    char * ret = "no greeting";
    return ret;
}

char * find_and_attibute_free_view(struct aquarium *aquarium, struct view * client_view){
    for(int i=0 ; i < aquarium->views_len ; i++){
        if(is_view_free(aquarium->views[i])){
            change_view_status(aquarium->views[i]);
            change_view_status(client_view);
            strcpy(client_view->name, aquarium->views[i]->name);
            return aquarium->views[i]->name;                
        }
    }
    char * ret = "no greeting";
    return ret;
}

int hello_command_check(char * buffer, char * view_name){
    char input[256];
    memcpy(input, buffer, 256);
    view_name = strtok(input," ");
    view_name = strtok(NULL," ");
    if(strcmp(view_name,"in")!=0){
        return 0;
    }
    view_name = strtok(NULL," ");
    if(strcmp(view_name,"as")!=0){
        return 0;
    }
    view_name = strtok(NULL," ");
    view_name = strtok(view_name,"\n");
    return 1;
}

char * get_fish(struct aquarium *aquarium, char * fish_name){(void) aquarium; (void) fish_name; char * ok = "ok"; return ok;}

void controller_update_fishes(struct aquarium * aquarium, int refresh_time) {
    update_fishes(aquarium, refresh_time);
}

int controller_start_fish(struct aquarium * aquarium, char * fish_name, int time_to_dest) {
    if (fish_name != NULL && time_to_dest != 0) {
        int val = start_fish(aquarium, fish_name, time_to_dest);
        return val;
    }
    return -1;
}

void controller_aquarium_print(struct aquarium * aquarium, char * to_print){
    char buffer [sizeof(int)*16+1];
    sprintf(buffer, "%d", aquarium->size[0]);
    strcat(to_print, buffer);
    strcat(to_print, "x");
    sprintf(buffer, "%d", aquarium->size[1]);
    strcat(to_print, buffer);
    strcat(to_print, "\n");
    for(int i=0 ; i<aquarium->views_len ; i++){
        strcat(to_print, aquarium->views[i]->name);
        strcat(to_print, " ");
        sprintf(buffer, "%d", aquarium->views[i]->coords[0]);
        strcat(to_print, buffer);
        strcat(to_print, "x");
        sprintf(buffer, "%d", aquarium->views[i]->coords[1]);
        strcat(to_print, buffer);
        strcat(to_print, "+");
        sprintf(buffer, "%d", aquarium->views[i]->size[0]);
        strcat(to_print, buffer);
        strcat(to_print, "+");
        sprintf(buffer, "%d", aquarium->views[i]->size[1]);
        strcat(to_print, buffer);
        strcat(to_print, "\n");
    }
    strcat(to_print, "\0");
}

int verif(char * buf, char * s, int client_count, pthread_mutex_t * mutex_client_count) {
    pthread_mutex_lock(mutex_client_count);
    if (client_count > 1) {
        pthread_mutex_unlock(mutex_client_count);
        return -1;
    }
    pthread_mutex_unlock(mutex_client_count);
    if (strcmp(buf, s) != 0) {
        return -1;
    }
    return 1;
}

void write_in_log(int check, const char * tag, int n, int number, const char * buf) {
    if (check == 1) {
        FILE *fp;
        char filename[256];
        strcpy(filename, "../Controller/log.txt");
        fp = fopen(filename, "a");

        if (fp == NULL) {
            printf("Failed to add in file\n");
            return;
        }
        
        if (strcmp(tag, "recv") == 0 && n == 0) {
            fprintf(fp, "Client %d déconnecté.\n", number);
        }
        else if (strcmp(tag, "recv") == 0 && n > 0 && buf != NULL) {
            fprintf(fp, "Message du client %d : %s\n", number, buf);
        }
        else if (strcmp(tag, "send") == 0 && buf != NULL) {
            fprintf(fp, "Message du serveur au client %d : %s\n", number, buf);
        }
        else if (strcmp(tag, "timeout") == 0 && buf != NULL) {
            fprintf(fp, "Client %d inactif pendant 30 secondes. Fermeture de la connexion.\nMessage du serveur au client %d : %s\n", number, number, buf);
        }
        else if (strcmp(tag, "occ") == 0 && buf != NULL) {
            fprintf(fp, "Le serveur est occupé. Impossible de gérer un nouveau client.\nMessage du serveur au client %d : %s\n", number, buf);
        }
        else if (strcmp(tag, "con") == 0 && buf != NULL) {
            fprintf(fp, "Client connecté. Client ID: %d\nMessage du serveur au client %d : %s\n", number, number, buf);
        }
        else if (strcmp(tag, "print") == 0 && buf != NULL) {
            fprintf(fp, "%s", buf);
        }
        else if (strcmp(tag, "unk") == 0 && buf != NULL) {
            fprintf(fp, "> Unknown command : should be \"load\", \"save\", \"show\", \"add view\", \"del view\" or \"exit\" (received %s)\n", buf);
        }
        else if (strcmp(tag, "prompt_send") == 0 && buf != NULL) {
            fprintf(fp, "%s\n", buf);
        }
        else if (strcmp(tag, "deco") == 0) {
            fprintf(fp, "Client %d déconnecté.\n", number);
        }

        fclose(fp);
    }
}

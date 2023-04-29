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

int load_initial_aquarium_config(const char *filename, struct aquarium *aquarium){
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
    aquarium_create(aquarium, size, "oui");

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

char * client_connection(struct aquarium *aquarium, char* view_name){
    if(view_name == NULL || view_name_check(aquarium, view_name)){
        return find_and_attibute_free_view(aquarium);
    }
    else{
        for(int i=0 ; i < aquarium->views_len ; i++){
            if(strcmp(aquarium->views[i]->name,view_name) == 0){
                if(is_view_free(aquarium->views[i])){
                    change_view_status(aquarium->views[i]);
                    return aquarium->views[i]->name;   
                }
                else{
                    return find_and_attibute_free_view(aquarium);
                }              
            }
        }
    }
    char * ret = "no greeting";
    return ret;
}

char * find_and_attibute_free_view(struct aquarium *aquarium){
    for(int i=0 ; i < aquarium->views_len ; i++){
        if(is_view_free(aquarium->views[i])){
            change_view_status(aquarium->views[i]);
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

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

int controller_add_fish(struct aquarium *aquarium, struct fish *fish){}

int controller_del_fish(struct aquarium *aquarium, char *fish){}

int client_connection(struct aquarium *aquarium, char* view_name){}

void ping(){}

char * get_fish(struct aquarium *aquarium, char * view_name){}
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

struct config {
    int controller_port;
    int display_timeout_value;
    int fish_update_interval;
};

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

int main()
{
    struct config conf;

    load_config("controller.cfg", &conf);

    /* Some tests*/
    printf("Controller port: %d\n", conf.controller_port);
    printf("Display timeout value: %d\n", conf.display_timeout_value);
    printf("Fish update interval: %d\n", conf.fish_update_interval);

    // server and socket file descriptor
    int server_fd, newsockfd;
    //TCP port
    int portno = 12345;
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Erreur lors de l'ouverture de la socket'");
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

    return 0;
} 
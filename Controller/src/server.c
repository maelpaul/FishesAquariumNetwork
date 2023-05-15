#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#include "server_utils.h"
#include "command_fish.h"
#include "command_getfish.h"
#include "prompt_commands.h"
#include "prompt.h"
#include "init_server.h"


#define NB_CLIENTS 8
#define BUFFER_SIZE 256

int client_count = 0;
int is_aquarium_loaded = 0;
pthread_t wait_client;
pthread_t threads[NB_CLIENTS];
struct client_args * tab_args[NB_CLIENTS];
pthread_mutex_t mutex_client_count = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_aquarium = PTHREAD_MUTEX_INITIALIZER;

struct wait_client_context {
    struct aquarium * aquarium;
    int server_fd;
};

/*
int verif(char * buf, char * s) {
    pthread_mutex_lock(&mutex_client_count);
    if (client_count > 1) {
        pthread_mutex_unlock(&mutex_client_count);
        return -1;
    }
    pthread_mutex_unlock(&mutex_client_count);
    if (strcmp(buf, s) != 0) {
        return -1;
    }
    return 1;
}*/

/*
void *thread_prompt(void *arg) {
    struct client_args *client_args = (struct client_args *) arg;
    int client_id = *(client_args->client_id);
    struct aquarium * aquarium = client_args->aquarium;
    int client_number = client_args->client_number;
    (void) aquarium;
    char buffer[BUFFER_SIZE];
    int n;
    int server_fd = *(client_args->fd_server);

    assert(client_number == 0);

    // Envoi d'un message au client
    strcpy(buffer, "> Bonjour prompt !");
    if (send(client_id, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }

    do {
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(client_id, buffer, sizeof(buffer), 0)) < 0) {
            perror("Erreur lors de la réception de la réponse du client");
            exit(EXIT_FAILURE);
        }
        prompt_load(buffer, aquarium, client_id, &is_aquarium_loaded, &mutex_aquarium); 
    } while(!is_aquarium_loaded);

    do {
        // Réception de la réponse du client
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(client_id, buffer, sizeof(buffer), 0)) < 0) {
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
            printf("Message du prompt (%d) : %s\n", client_number, buffer);
        }

        if (verif(buffer, "quit\n") != 1) {
            strcpy(buffer, "> NOK");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }
    } while(verif(buffer, "quit\n") != 1);

    strcpy(buffer, "> Bye");
    if (send(client_id, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mutex_client_count);

    for (int i = client_count-1; i > -1; i--) {
        close(*(tab_args[i]->client_id));
        free(tab_args[i]->client_id);
        free(tab_args[i]->fd_server);
        free(tab_args[i]);
    }

    client_count = 0;

    pthread_mutex_unlock(&mutex_client_count);

    pthread_mutex_lock(&mutex_aquarium);
    aquarium_free(aquarium);
    pthread_mutex_unlock(&mutex_aquarium);
    printf("Fermeture du serveur...\n");
    assert(close(server_fd) == 0);
    exit(EXIT_SUCCESS);

    pthread_exit(NULL);
}*/

void *thread_client(void *arg) {
    struct client_args *client_args = (struct client_args *) arg;
    int client_id = *(client_args->client_id);
    struct aquarium * aquarium = client_args->aquarium;
    int client_number = client_args->client_number;
    (void) aquarium;
    char buffer[BUFFER_SIZE];
    int n;

    // Envoi d'un message au client
    strcpy(buffer, "> Bonjour client ");
    char chaine[10];
    sprintf(chaine, "%d !\n", client_number);
    strcat(buffer, chaine);
    if (send(client_id, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }

    while(is_aquarium_loaded == 0) {
        continue;
    }

    do {
        int check = 0;

        // Réception de la réponse du client
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(client_id, buffer, sizeof(buffer), 0)) < 0) {
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
            printf("Message du client %d : %s\n", client_number, buffer);
        }

        if (check == 0) {
            check = add_fish_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = del_fish_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = start_fish_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = get_fish_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = get_fish_continuously_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = get_status_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = ping_server(buffer, client_id);
        }
        if (check == 0) {
            check = init_server(buffer, aquarium, &mutex_aquarium, client_id);
        }

        // check des commandes inexistantes
        if (check == 0 && strcmp(buffer, "log out\n") != 0) {
            strcpy(buffer, "> NOK : Inexisting command");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }
    } while(strcmp(buffer, "log out\n") != 0);

    strcpy(buffer, "> Bye");
    if (send(client_id, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }

    // Fermer la connexion avec le client
    close(client_id);
    free(client_args->client_id);
    free(client_args->fd_server);
    free(arg);

    pthread_mutex_lock(&mutex_client_count);
    client_count--;
    pthread_mutex_unlock(&mutex_client_count);

    pthread_exit(NULL);
}

void * wait_for_client(void * arg){
    socklen_t client_addr_len;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    int newsockfd;

    struct wait_client_context * context = (struct wait_client_context *) arg;

    while(1) {
        // Accepter une connexion entrante
        client_addr_len = sizeof(client_addr);
        newsockfd = accept(context->server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (newsockfd == -1) {
            perror("Erreur lors de l'acceptation de la connexion");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&mutex_client_count);

        if (client_count < NB_CLIENTS) {
            // Créer un thread pour gérer le client
            int *fd_server = malloc(sizeof(int));
            *fd_server = context->server_fd;
            int *client_id = malloc(sizeof(int));
            *client_id = newsockfd;
            struct client_args * client_args = malloc(sizeof(struct client_args));
            client_args->aquarium = context->aquarium;
            client_args->client_id = client_id;
            client_args->client_number = client_count;
            client_args->fd_server = fd_server;
            pthread_create(&threads[client_count], NULL, thread_client, (void *)client_args);
            printf("Client connected. Client ID: %d\n", client_count);
            tab_args[client_count] = client_args;
            client_count++;
        } 
        else {
            // Le nombre maximal de clients est atteint
            printf("Le serveur est occupé. Impossible de gérer un nouveau client.\n");
            
            strcpy(buffer, "> Bye");
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }

            close(newsockfd);
        }

        pthread_mutex_unlock(&mutex_client_count);
    }
}

int main()
{
    struct config conf;

    load_config("controller.cfg", &conf);

    struct aquarium * aquarium = malloc(sizeof(struct aquarium));

    // server and socket file descriptor
    int server_fd;
    int portno = conf.controller_port;
    struct sockaddr_in serv_addr;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Erreur lors de l'ouverture de la socket'");
        exit(EXIT_FAILURE);
    }

    const int enable = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
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
    if (listen(server_fd, NB_CLIENTS) < 0) {
        perror("Erreur lors de la mise en écoute du socket");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexions...\n");

    struct wait_client_context context;
    context.aquarium = aquarium;
    context.server_fd = server_fd;

    pthread_create(&wait_client, NULL, wait_for_client, (void *) &context);
    
    prompt(&is_aquarium_loaded, aquarium, &mutex_aquarium);

    pthread_join(wait_client, NULL);

    // Attendre que tous les threads se terminent
    for (int i = 0; i < client_count; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_lock(&mutex_aquarium);
    aquarium_free(aquarium);
    pthread_mutex_unlock(&mutex_aquarium);
    close(server_fd);

    return 0;
}

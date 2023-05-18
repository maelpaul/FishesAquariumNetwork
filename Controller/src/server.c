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
#include "init_client.h"


#define NB_CLIENTS 8
#define BUFFER_SIZE 256

int client_count = 0;
int is_aquarium_loaded = 0;
pthread_t wait_client;
pthread_t threads[NB_CLIENTS];
struct client_args * tab_args[NB_CLIENTS];
pthread_mutex_t mutex_client_count = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_aquarium = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_is_aquarium_loaded = PTHREAD_MUTEX_INITIALIZER; 

struct wait_client_context {
    struct aquarium * aquarium;
    int server_fd;
};

void *thread_client(void *arg) {
    struct client_args *client_args = (struct client_args *) arg;
    int client_id = *(client_args->client_id);
    pthread_mutex_lock(&mutex_aquarium);
    struct aquarium * aquarium = client_args->aquarium;
    pthread_mutex_unlock(&mutex_aquarium);
    int client_number = client_args->client_number;
    struct view * client_view = client_args->client_view;
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

    pthread_mutex_lock(&mutex_is_aquarium_loaded);
    while(is_aquarium_loaded == 0) {
        pthread_mutex_unlock(&mutex_is_aquarium_loaded);
        sleep(1);
        pthread_mutex_lock(&mutex_is_aquarium_loaded);
    }
    pthread_mutex_unlock(&mutex_is_aquarium_loaded);

    do {
        int check = 0;

        // Réception de la réponse du client
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(client_id, buffer, sizeof(buffer), 0)) < 0) {
            perror("Erreur lors de la réception de la réponse du client");
            exit(EXIT_FAILURE);
        }
        printf("Message du client %d , len(%ld) : %s\n", client_number,  strlen(buffer), buffer);

        int test = 0;
        for(long unsigned int i=0;i<5;i++){
            if (buffer[test]=='\0'){
                test++;
            }
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
            check = get_fish_continuously_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = get_fish_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = get_status_server(buffer, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0) {
            check = ping_server(buffer, client_id);
        }
        if (check == 0) {
            check = init_client(buffer, aquarium, &mutex_aquarium, client_id, client_view);
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

    for (int i = 0; i < aquarium->views_len; i++) {
        if (strcmp(client_view->name, aquarium->views[i]->name) == 0) {
            change_view_status(aquarium->views[i]);
            change_view_status(client_args->client_view);
        }
    }
    
    view_free(client_args->client_view);
    free(client_args->client_id);
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
            int *client_id = malloc(sizeof(int));
            *client_id = newsockfd;
            struct client_args * client_args = malloc(sizeof(struct client_args));
            pthread_mutex_lock(&mutex_aquarium);
            client_args->aquarium = context->aquarium;
            client_args->client_id = client_id;
            client_args->client_number = client_count+1;
            client_args->client_view = malloc(sizeof(struct view));
            view_init(client_args->client_view);
            int size[2] = {0, 0};
            int coords[2] = {0, 0};
            view_create(client_args->client_view, coords, size, "nok", client_args->aquarium->size[0], client_args->aquarium->size[1]);
            pthread_mutex_unlock(&mutex_aquarium);
            pthread_create(&threads[client_count], NULL, thread_client, (void *)client_args);
            printf("Client connecté. Client ID: %d\n", client_count+1);
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
    char buffer[BUFFER_SIZE];

    load_config("controller.cfg", &conf);

    struct aquarium * aquarium = malloc(sizeof(struct aquarium));
    aquarium_init(aquarium);

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
    pthread_mutex_lock(&mutex_aquarium);
    context.aquarium = aquarium;
    pthread_mutex_unlock(&mutex_aquarium);
    context.server_fd = server_fd;

    pthread_create(&wait_client, NULL, wait_for_client, (void *) &context);
    
    prompt(&is_aquarium_loaded, aquarium, &mutex_aquarium, &mutex_is_aquarium_loaded);
    
    pthread_mutex_lock(&mutex_client_count);
    pthread_mutex_lock(&mutex_aquarium);
    pthread_mutex_lock(&mutex_is_aquarium_loaded);

    pthread_cancel(wait_client);
    pthread_join(wait_client, NULL);

    // Attendre que tous les threads se terminent
    for (int i = 0; i < client_count; i++) {
        strcpy(buffer, "> Serveur fermé");
        if (send(*(tab_args[i]->client_id), buffer, strlen(buffer), 0) < 0) {
            perror("Erreur lors de l'envoi du message au client");
            exit(EXIT_FAILURE);
        }
        close(*(tab_args[i]->client_id));

        for (int i = 0; i < aquarium->views_len; i++) {
            if (strcmp(tab_args[i]->client_view->name, aquarium->views[i]->name) == 0) {
                change_view_status(aquarium->views[i]);
                change_view_status(tab_args[i]->client_view);
            }
        }
        
        view_free(tab_args[i]->client_view);
        free(tab_args[i]->client_id);
        free(tab_args[i]);
        pthread_cancel(threads[i]);
        pthread_join(threads[i], NULL);
    }

    client_count = 0;
    aquarium_free(aquarium);

    pthread_mutex_unlock(&mutex_is_aquarium_loaded);
    pthread_mutex_unlock(&mutex_aquarium);
    pthread_mutex_unlock(&mutex_client_count);
    
    printf("Fermeture du serveur...\n");
    close(server_fd);

    return 0;
}

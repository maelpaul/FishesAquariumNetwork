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
#include <sys/select.h>

#include "server_utils.h"
#include "command_fish.h"
#include "command_getfish.h"
#include "prompt_commands.h"
#include "prompt.h"
#include "init_client.h"


#define NB_CLIENTS 8
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

int client_count = 0;
int is_aquarium_loaded = 0;
pthread_t wait_client;
pthread_t threads[NB_CLIENTS];
int available[NB_CLIENTS];
struct client_args * tab_args[NB_CLIENTS];
pthread_mutex_t mutex_client_count = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_aquarium = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_is_aquarium_loaded = PTHREAD_MUTEX_INITIALIZER;
int print_client_answer = 1; 
int my_log = 0;
int has_view[NB_CLIENTS];

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
    int thread_number = client_args->thread_number;
    struct view * client_view = client_args->client_view;
    (void) aquarium;
    char buffer[BUFFER_SIZE];
    int n;

    // Envoi d'un message au client
    strcpy(buffer, "-1|> Bonjour client ");
    char chaine[10];
    sprintf(chaine, "%d !\n", client_number);
    strcat(buffer, chaine);
    if (send(client_id, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }
    write_in_log(my_log, "send", 0, client_number, buffer);

    pthread_mutex_lock(&mutex_is_aquarium_loaded);
    while(is_aquarium_loaded == 0) {
        pthread_mutex_unlock(&mutex_is_aquarium_loaded);
        sleep(1);
        pthread_mutex_lock(&mutex_is_aquarium_loaded);
    }
    pthread_mutex_unlock(&mutex_is_aquarium_loaded);

    char * header;
    char * message;
    time_t last_communication_time;
    time(&last_communication_time);

    do {
        int check = 0;

        // Utilisation de select pour surveiller les messages des clients pendant 30 secondes
        fd_set read_fds;
        struct timeval timeout;
        FD_ZERO(&read_fds);
        FD_SET(client_id, &read_fds);
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;

        int select_result = select(client_id + 1, &read_fds, NULL, NULL, &timeout);
        if (select_result == -1) {
            perror("Erreur lors de l'utilisation de select");
            exit(EXIT_FAILURE);
        } else if (select_result == 0) {
            printf("Client %d inactif pendant 30 secondes. Fermeture de la connexion.\n", client_number);
            strcpy(buffer, "-1|Timeout");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(my_log, "timeout", n, client_number, buffer);
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
            available[thread_number] = 1;
            pthread_mutex_unlock(&mutex_client_count);

            pthread_exit(NULL);
        }

        // Réception de la réponse du client
        memset(buffer, 0, sizeof(buffer));
        if ((n = recv(client_id, buffer, sizeof(buffer), 0)) < 0) {
            perror("Erreur lors de la réception de la réponse du client");
            exit(EXIT_FAILURE);
        }
        else if (n == 0) {
            printf("Client %d déconnecté.\n", client_number);
            write_in_log(my_log, "recv", n, client_number, buffer);
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
            available[thread_number] = 1;
            pthread_mutex_unlock(&mutex_client_count);

            pthread_exit(NULL);
        }
        if (print_client_answer == 1) {
            printf("Message du client %d : %s\n", client_number, buffer);
        }
        write_in_log(my_log, "recv", n, client_number, buffer);

        int test = 0;
        for(long unsigned int i=0;i<5;i++){
            if (buffer[test]=='\0'){
                test++;
            }
        }
        
        header = strtok(buffer, "|");
        message = strtok(NULL, "\0");
        int val = -1;

        if (check == 0 && has_view[client_number] == 1) {
            check = add_fish_server(my_log, client_number, header, message, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0 && has_view[client_number] == 1) {
            check = del_fish_server(my_log, client_number, header, message, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0 && has_view[client_number] == 1) {
            check = start_fish_server(my_log, client_number, header, message, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0 && has_view[client_number] == 1) {
            check = get_fish_ls_server(my_log, client_number, header, message, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0 && has_view[client_number] == 1) {
            val = get_fish_continuously_server(my_log, client_number, header, message, aquarium, &mutex_aquarium, client_id, BUFFER_SIZE);
            if (val == 1) {
                printf("Client %d déconnecté.\n", client_number);
                write_in_log(my_log, "recv", n, client_number, buffer);
            
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
                available[thread_number] = 1;
                pthread_mutex_unlock(&mutex_client_count);

                pthread_exit(NULL);
            }
        }
        if (check == 0 && val == 0 && has_view[client_number] == 1) {
            check = get_fish_server(my_log, client_number, header, message, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0 && has_view[client_number] == 1) {
            check = get_status_server(my_log, client_number, header, message, aquarium, &mutex_aquarium, client_id);
        }
        if (check == 0 && has_view[client_number] == 1) {
            check = ping_server(my_log, client_number, header, message, client_id);
        }
        if (check == 0) {
            check = init_client(my_log, client_number, header, message, aquarium, &mutex_aquarium, client_id, client_view);
            if (check == 1 && is_view_free(client_view) == 0) {
                has_view[client_number] = 1;
            }
        }

        // check des commandes inexistantes
        if (check == 0 && val == 0 && strcmp(message, "log out\n") != 0) {
            strcpy(buffer, header);
            strcat(buffer, "|NOK : Inexisting command");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(my_log, "send", n, client_number, buffer); 
        }

        if (check == 0 && (has_view[client_number] == 0 || val == 2)) {
            strcpy(buffer, header);
            strcat(buffer, "|NOK : Forbidden command");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(my_log, "send", n, client_number, buffer); 
        }
    } while(strcmp(message, "log out\n") != 0);

    strcpy(buffer, "-1|Bye");
    if (send(client_id, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi du message au client");
        exit(EXIT_FAILURE);
    }
    write_in_log(my_log, "send", n, client_number, buffer);
    printf("Client %d déconnecté.\n", client_number);
    write_in_log(my_log, "deco", n, client_number, buffer);

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
    available[thread_number] = 1;
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
            int number;

            // Créer un thread pour gérer le client
            int first_available = -1;
            int ok = 0;
            for (int i = 0; i < NB_CLIENTS; i++) {
                if (available[i] == 1 && ok == 0) {
                    ok = 1;
                    first_available = i;
                    number = first_available+1;
                    available[i] = 0;
                }
            }

            int *client_id = malloc(sizeof(int));
            *client_id = newsockfd;
            struct client_args * client_args = malloc(sizeof(struct client_args));
            pthread_mutex_lock(&mutex_aquarium);
            client_args->aquarium = context->aquarium;
            client_args->client_id = client_id;
            client_args->client_number = number;
            client_args->thread_number = first_available;
            client_args->client_view = malloc(sizeof(struct view));
            view_init(client_args->client_view);
            int size[2] = {0, 0};
            int coords[2] = {0, 0};
            view_create(client_args->client_view, coords, size, "nok", client_args->aquarium->size[0], client_args->aquarium->size[1]);
            pthread_mutex_unlock(&mutex_aquarium);
            pthread_create(&threads[first_available], NULL, thread_client, (void *)client_args);
            printf("Client connecté. Client ID: %d\n", number);
            write_in_log(my_log, "con", 0, number, NULL);
            tab_args[first_available] = client_args;
            client_count++;
        } 
        else {
            // Le nombre maximal de clients est atteint
            printf("Le serveur est occupé. Impossible de gérer un nouveau client.\n");
            
            strcpy(buffer, "-1|Bye");
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(my_log, "occ", 0, 0, buffer);

            close(newsockfd);
        }

        pthread_mutex_unlock(&mutex_client_count);
    }
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-m") == 0) {
            print_client_answer = 0;
        }
        if (strcmp(argv[i], "-h") == 0) {
            printf("*** May the Force be with you ***\n");
            write_in_log(my_log, "print", 0, 0, "*** May the Force be with you ***\n");
        }
        if (strcmp(argv[i], "-l") == 0) {
            my_log = 1;

            FILE *fp;
            char filename[256];
            strcpy(filename, "../Controller/log.txt");
            fp = fopen(filename, "w+");

            if (fp == NULL) {
                printf("Failed to create file\n");
                exit(EXIT_FAILURE);
            }

            fclose(fp);
        }
    }

    for (int i = 0; i < NB_CLIENTS; i++) {
        available[i] = 1;
        has_view[i] = 0;
    }

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
    write_in_log(my_log, "print", 0, 0, "Serveur en attente de connexions...\n");

    struct wait_client_context context;
    pthread_mutex_lock(&mutex_aquarium);
    context.aquarium = aquarium;
    pthread_mutex_unlock(&mutex_aquarium);
    context.server_fd = server_fd;

    pthread_create(&wait_client, NULL, wait_for_client, (void *) &context);
    
    prompt(my_log, &is_aquarium_loaded, aquarium, &mutex_aquarium, &mutex_is_aquarium_loaded);
    
    pthread_mutex_lock(&mutex_client_count);
    pthread_mutex_lock(&mutex_aquarium);
    pthread_mutex_lock(&mutex_is_aquarium_loaded);

    pthread_cancel(wait_client);
    pthread_join(wait_client, NULL);

    // Attendre que tous les threads se terminent
    for (int i = 0; i < NB_CLIENTS; i++) {
        if (available[i] == 0) {
            strcpy(buffer, "-1|Serveur fermé");
            if (send(*(tab_args[i]->client_id), buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
            write_in_log(my_log, "send", 0, i+1, buffer);

            close(*(tab_args[i]->client_id));

            for (int j = 0; j < aquarium->views_len; j++) {
                if (strcmp(tab_args[i]->client_view->name, aquarium->views[j]->name) == 0) {
                    change_view_status(aquarium->views[j]);
                    change_view_status(tab_args[i]->client_view);
                }
            }
            
            view_free(tab_args[i]->client_view);
            free(tab_args[i]->client_id);
            free(tab_args[i]);
            pthread_cancel(threads[i]);
            pthread_join(threads[i], NULL);
            available[i] = 1;
        }
    }

    client_count = 0;
    aquarium_free(aquarium);

    pthread_mutex_unlock(&mutex_is_aquarium_loaded);
    pthread_mutex_unlock(&mutex_aquarium);
    pthread_mutex_unlock(&mutex_client_count);
    
    printf("Fermeture du serveur...\n");
    write_in_log(my_log, "print", 0, 0, "Fermeture du serveur...\n");
    close(server_fd);

    return 0;
}

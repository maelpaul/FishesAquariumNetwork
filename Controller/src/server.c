#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#include "server_utils.h"
#include "command_fish.h"
#include "command_getfish.h"

#define NB_CLIENTS 8
#define BUFFER_SIZE 256

int client_count = 0;
int is_aquarium_loaded = 0;
pthread_t threads[NB_CLIENTS];
struct client_args * tab_args[NB_CLIENTS];
pthread_mutex_t mutex_client_count = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_aquarium = PTHREAD_MUTEX_INITIALIZER;

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
}

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
        int test = 0;
        for(long unsigned int i=0;i<5;i++){
            if (buffer[test]=='\0'){
                test++;
            }
        }
        if (test<=2) {
            printf("Message du prompt (%d) : %s\n", client_number, buffer);
        }

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
                pthread_mutex_lock(&mutex_aquarium);
                load_initial_aquarium_config("aquarium_example.txt", aquarium);
                pthread_mutex_unlock(&mutex_aquarium);
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

                is_aquarium_loaded=1;

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
        else {
            strcpy(buffer, "> NOK : you first need to load the aquarium");
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE); 
            }
        }  
    } while(is_aquarium_loaded == 0);

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
}

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

int main()
{
    struct config conf;
    char buffer[BUFFER_SIZE];

    load_config("controller.cfg", &conf);

    struct aquarium * aquarium = malloc(sizeof(struct aquarium));

    // server and socket file descriptor
    int server_fd, newsockfd;
    int portno = conf.controller_port;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_len;
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

    while(1) {
        // Accepter une connexion entrante
        client_addr_len = sizeof(client_addr);
        newsockfd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (newsockfd == -1) {
            perror("Erreur lors de l'acceptation de la connexion");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&mutex_client_count);

        if (client_count < NB_CLIENTS) {
            // Créer un thread pour gérer le client
            int *fd_server = malloc(sizeof(int));
            *fd_server = server_fd;
            int *client_id = malloc(sizeof(int));
            *client_id = newsockfd;
            struct client_args * client_args = malloc(sizeof(struct client_args));
            client_args->aquarium = aquarium;
            client_args->client_id = client_id;
            client_args->client_number = client_count;
            client_args->fd_server = fd_server;
            if (client_count == 0) {
                pthread_create(&threads[client_count], NULL, thread_prompt, (void *)client_args);
                printf("Prompt connected. Prompt ID: %d\n", client_count);
            }
            else {
                pthread_create(&threads[client_count], NULL, thread_client, (void *)client_args);
                printf("Client connected. Client ID: %d\n", client_count);
            }
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

    // Attendre que tous les threads se terminent
    for (int i = 0; i < client_count; i++) {
        pthread_join(threads[i], NULL);
    }

    aquarium_free(aquarium);
    close(server_fd);

    return 0;
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    // do {
    //     int check = 0;

    //     // Réception de la réponse du client
    //     memset(buffer, 0, sizeof(buffer));
    //     if ((n = recv(newsockfd, buffer, sizeof(buffer), 0)) < 0) {
    //         perror("Erreur lors de la réception de la réponse du client");
    //         exit(EXIT_FAILURE);
    //     }
    //     int test = 0;
    //     for(long unsigned int i=0;i<5;i++){
    //         if (buffer[test]=='\0'){
    //             test++;
    //         }
    //     }
    //     if (test<=2) {
    //         printf("Message du client : %s\n", buffer);
    //     }

    //     // command from prompt
    //     char load_verif[5];
    //     strncpy (load_verif, buffer, 4);
    //     load_verif[4] = '\0';
    //     if (!strcmp(load_verif, "load")) {
    //         check = 1;
            
    //         char info[256];
    //         memcpy(info, buffer, 256);
    //         char delim[] = " ";

    //         char * verif = strtok(info, delim);
    //         (void) verif;
    //         char * _aquarium_name = strtok(NULL, delim);
    //         char * aquarium_name = strtok(_aquarium_name, "\n");

    //         if (!strcmp(aquarium_name, "aquarium")) {
    //             strcpy(buffer, "> OK : aquarium already loaded");
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //         else {
    //             strcpy(buffer, "> NOK : aquarium not existing");
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //     }

    //     // Initialisation et authentification
    //     if (!strncmp(buffer,"hello",5)) {
    //         check = 1;
    //         char * view_name = NULL;
    //         int x = 1;
    //         if(strlen(buffer)!=6){
    //             x = hello_command_check(buffer, view_name);
    //         }
    //         if(x){
    //             char * attributed_view = client_connection(aquarium, view_name);
    //             if(strcmp(attributed_view,"no greeting")==0){
    //                 char to_send[64] = "> ";
    //                 strcat(to_send, attributed_view);
    //                 strcpy(buffer, to_send);
    //                 if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                     perror("Erreur lors de l'envoi du message au client");
    //                     exit(EXIT_FAILURE);
    //                 }
    //             }
    //             else{
    //                 char to_send[64] = "> greeting ";
    //                 strcat(to_send, attributed_view);
    //                 strcpy(buffer, to_send);
    //                 if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                     perror("Erreur lors de l'envoi du message au client");
    //                     exit(EXIT_FAILURE);
    //                 } 
    //             }
    //         }
    //         else{
    //             char to_send[64] = "> incorrect format";
    //             strcpy(buffer, to_send);
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             } 
    //         }
    //     }

    //     // Demande continue de Poisson
    //     int continuous = 0;
    //     char ask_continuous_verif[22];
    //     char ls[3];
    //     strncpy (ask_continuous_verif , buffer, 21);
    //     strncpy (ls , buffer, 2);
    //     ask_continuous_verif[21] = '\0';   /* null character manually added */
    //     ls[2] = '\0';

    //     if (!strcmp(ask_continuous_verif, "getFishesContinuously") || !strcmp(ls, "ls")) {
    //         continuous = 1;
    //         check = 1;
    //         // Lister les poissons en continue
    //         for (int i = 0; i < 10; ++i) {
    //             controller_update_fishes(aquarium, REFRESH_TIME);
    //             char fish_list[1024] = "> list ";
    //             for (int i = 0; i < aquarium->fishes_len; i++) {
    //                 char fish_info[128];
    //                 sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, aquarium->fishes[i]->dest[0], aquarium->fishes[i]->dest[1], aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->time_to_dest);
    //                 strcat(fish_list, fish_info);
    //             }
    //             strcat(fish_list, "\n");
    //             if (send(newsockfd, fish_list, strlen(fish_list), 0) < 0) {
    //                 perror("Erreur lors de l'envoi de la liste des poissons au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //             if (i != 9) {
    //                 sleep(1);
    //             }                
    //         }
    //     }

    //     // Demande périodique des poissons
    //     if (continuous == 0) {
    //         char ask_periodic_verif[10];
    //         strncpy (ask_periodic_verif , buffer, 10);
    //         ask_periodic_verif[9] = '\0';   /* null character manually added */

    //         if (!strcmp(ask_periodic_verif, "getFishes")) {
    //             check = 1;
    //             controller_update_fishes(aquarium, REFRESH_TIME);
    //             char fish_list[1024] = "> list ";
    //             for (int i = 0; i < aquarium->fishes_len; i++) {
    //                 char fish_info[128];
    //                 sprintf(fish_info, "[%s at %dx%d,%dx%d,%d] ", aquarium->fishes[i]->name, aquarium->fishes[i]->dest[0], aquarium->fishes[i]->dest[1], aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->time_to_dest);
    //                 strcat(fish_list, fish_info);
    //             }
    //             strcat(fish_list, "\n");
    //             if (send(newsockfd, fish_list, strlen(fish_list), 0) < 0) {
    //                 perror("Erreur lors de l'envoi de la liste des poissons au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //     }


    //     // statut
    //     char status_verif[7];
    //     strncpy (status_verif, buffer, 6);
    //     status_verif[6] = '\0';   /* null character manually added */

    //     if (!strcmp(status_verif, "status")) {
    //         check = 1;
    //         controller_update_fishes(aquarium, REFRESH_TIME);
    //         // Affichage statut
    //         int nb_fishes = aquarium->fishes_len;
    //         char len[10];
    //         sprintf(len, "%d ", nb_fishes);
    //         char info[1024] = "> OK : Connecté au contrôleur, ";
    //         strcat(info, len);

    //         if (nb_fishes == 0 || nb_fishes == 1) {
    //             char info2[100] = "poisson trouvé";
    //             strcat(info, info2);
    //         }
    //         else {
    //             char info2[100] = "poissons trouvés";
    //             strcat(info, info2);
    //         }
    //         strcat(info, "\n");

    //         for (int i = 0; i < aquarium->fishes_len; i++) {
    //             char fish_info[128];
    //             sprintf(fish_info, "Fish %s at %dx%d, %dx%d ", aquarium->fishes[i]->name, aquarium->fishes[i]->coords[0], aquarium->fishes[i]->coords[1], aquarium->fishes[i]->size[0], aquarium->fishes[i]->size[1]);
    //             if (aquarium->fishes[i]->started == 1) {
    //                 strcat(fish_info, "started");
    //             }
    //             else {
    //                 strcat(fish_info, "notStarted");
    //             }
    //             strcat(fish_info, "\n");
    //             strcat(info, fish_info);
    //         }
    //         if (send(newsockfd, info, strlen(info), 0) < 0) {
    //             perror("Erreur lors de l'envoi de la liste des poissons au client");
    //             exit(EXIT_FAILURE);
    //         }
    //     }

    //     // ping
    //     char ping_verif[5];
    //     strncpy (ping_verif, buffer, 4);
    //     ping_verif[4] = '\0';   /* null character manually added */

    //     if (!strcmp(ping_verif, "ping")) {
    //         check = 1;

    //         char info[256];
    //         memcpy(info, buffer, 256);
    //         char delim[] = " ";

    //         char * verif = strtok(info, delim);
    //         (void) verif;
    //         char * _channel = strtok(NULL, delim);
    //         char * channel = strtok(_channel, "\n");

    //         if (channel == NULL) {
    //             strcpy(buffer, "> NOK : numéro de port invalide");
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //         else if (!strcmp(channel, "12345")) {
    //             strcpy(buffer, "> pong 12345");
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //         else {
    //             strcpy(buffer, "> NOK : mauvais numéro de port");
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //     }

    //     // show command from prompt
    //     char show_verif[5];
    //     strncpy (show_verif, buffer, 4);
    //     show_verif[4] = '\0';
    //     if (!strcmp(show_verif, "show")) {
    //         check = 1;

    //         char info[256];
    //         memcpy(info, buffer, 256);
    //         char delim[] = " ";

    //         char * verif = strtok(info, delim);
    //         (void) verif;
    //         char * _aquarium_name = strtok(NULL, delim);
    //         char * aquarium_name = strtok(_aquarium_name, "\n");

    //         if (!strcmp(aquarium_name, "aquarium")) {
    //             char to_send[256] = "> OK : \n";
    //             controller_aquarium_print(aquarium, to_send);
    //             strcpy(buffer, to_send);
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //         else {
    //             strcpy(buffer, "> NOK : aquarium not existing");
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //     }

    //     // del view command from prompt
    //     char del_view_verif[9];
    //     strncpy (del_view_verif, buffer, 9);
    //     del_view_verif[8] = '\0';
    //     if (!strcmp(del_view_verif, "del view")) {
    //         check = 1;
    //         char info[256];
    //         memcpy(info, buffer, 256);
    //         char delim[] = " ";

    //         char * view_name = strtok(info, delim);
    //         view_name = strtok(NULL, delim);
    //         view_name = strtok(NULL, "\n");
            
    //         int removed = 0;
    //         for(int i=0; i<aquarium->views_len; i++){
    //             if(strcmp(aquarium->views[i]->name,view_name)==0){
    //                 del_view(aquarium, view_name);
    //                 removed = 1;
    //                 char to_send[256] = "> OK : view ";
    //                 strcat(to_send, view_name);
    //                 strcat(to_send, " deleted.");
    //                 strcpy(buffer, to_send);
    //                 if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                     perror("Erreur lors de l'envoi du message au client");
    //                     exit(EXIT_FAILURE);
    //                 }
    //                 break;
    //             }
    //         }
    //         if(!removed){
    //             strcpy(buffer, "> NOK : view not existing");
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //     }

    //     // add view command from prompt
    //     char add_view_verif[9];
    //     strncpy (add_view_verif, buffer, 9);
    //     add_view_verif[8] = '\0';
    //     if (!strcmp(add_view_verif, "add view")) {
    //         check = 1;
    //         char info[256];
            
    //         memcpy(info, buffer, 256);
    //         char delim[] = " ";

    //         char * view_name = strtok(info, delim);
    //         view_name = strtok(NULL, delim);
    //         view_name = strtok(NULL, delim);
    //         int coords[2];
    //         coords[0]=atoi(strtok(NULL,"x"));
    //         coords[1]=atoi(strtok(NULL,"+"));
    //         int size[2];
    //         size[0]=atoi(strtok(NULL,"+"));
    //         size[1]=atoi(strtok(NULL,"\n"));

    //         if(view_name_check(aquarium, view_name)){
    //             strcpy(buffer, "> NOK : View already exists");
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             } 
    //         }
    //         else{
    //             add_view(aquarium, coords, size, view_name);
    //             char to_send[256] = "> OK : view ";
    //             strcat(to_send, view_name);
    //             strcat(to_send, " added.");
    //             strcpy(buffer, to_send);
    //             if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
    //                 perror("Erreur lors de l'envoi du message au client");
    //                 exit(EXIT_FAILURE);
    //             }
    //         }
    //     }
    /*
        char save_verif[5];
        strncpy (save_verif, buffer, 5);
        save_verif[4] = '\0';
        if (!strcmp(save_verif, "save")) {
            check = 1;
            char info[256];
            
            memcpy(info, buffer, 256);
            char delim[] = " ";

            char * aquarium_name = strtok(info, delim);
            aquarium_name = strtok(NULL, "\n");

            if (!strcmp(aquarium_name, "aquarium")) {
                //save_aquarium(aquarium); segfault

                strcpy(buffer, "> OK : aquarium saved");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else {
                strcpy(buffer, "> NOK : aquarium not existing");
                if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                    }
                }
            }
        
        // check des commandes inexistantes
        if (check == 0 && strcmp(buffer, "log out\n") != 0) {
            strcpy(buffer, "> NOK : Inexisting command");
            if (send(newsockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }

    } while(strcmp(buffer, "log out\n") != 0);

    // Envoi de sortie de connexion au client
} */

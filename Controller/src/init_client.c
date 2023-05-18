#include "init_client.h"

char to_send[64];

int init_client(char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, struct view * client_view) {
    if (!strncmp(buffer,"hello",5)) {
        if (strcmp(client_view->name, "nok") != 0) {
            strcpy(to_send,header);
            strcat(to_send, "|> you already have a view : ");
            strcat(to_send, client_view->name);
            strcat(to_send, "\n");
            strcpy(buffer, to_send);
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
            return 1;
        }
        char * view_name = NULL;
        int x = 2;

        if(strlen(buffer) != 6)
            x = hello_command_check(buffer, view_name);
        

        if(x){
            if (x != 2) {
                char input[256];
                memcpy(input, buffer, 256);
                view_name = strtok(input," ");
                view_name = strtok(NULL," ");
                view_name = strtok(NULL," ");
                view_name = strtok(NULL," ");
                view_name = strtok(view_name,"\n");
            }

            pthread_mutex_lock(mutex);
            char * attributed_view = client_connection(aquarium, view_name, client_view);
            pthread_mutex_unlock(mutex);
            
            if(strcmp(attributed_view,"no greeting")==0){
                strcpy(to_send, header);
                strcpy(to_send, "|> ");
                strcat(to_send, attributed_view);
                if (send(client_id, to_send, strlen(to_send), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                strcpy(to_send, header);
                strcat(to_send, "|> greeting ");
                strcat(to_send, attributed_view);
                if (send(client_id, to_send, strlen(to_send), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                } 
            }
        }
        else{
            strcpy(to_send, header);
            strcat(to_send, "|> incorrect format");
            if (send(client_id, to_send, strlen(to_send), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }
        return 1;
    }
    return 0;
}

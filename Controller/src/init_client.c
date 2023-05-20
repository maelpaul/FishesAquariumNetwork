#include "init_client.h"

char init_client_buffer[64];

int init_client(int check_ls, int client_number, char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int client_id, struct view * client_view) {
    if (!strncmp(buffer,"hello",5)) {
        if (strcmp(client_view->name, "nok") != 0) {
            strcpy(init_client_buffer,header);
            strcat(init_client_buffer, "|you already have a view : ");
            strcat(init_client_buffer, client_view->name);
            strcat(init_client_buffer, "\n");
            strcpy(buffer, init_client_buffer);
            if (send(client_id, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
            write_in_log(check_ls, "send", 0, client_number, buffer);
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
                strcpy(init_client_buffer, header);
                strcpy(init_client_buffer, "|");
                strcat(init_client_buffer, attributed_view);
                if (send(client_id, init_client_buffer, strlen(init_client_buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                }
                write_in_log(check_ls, "send", 0, client_number, init_client_buffer);
            }
            else{
                strcpy(init_client_buffer, header);
                strcat(init_client_buffer, "|greeting ");
                strcat(init_client_buffer, attributed_view);
                if (send(client_id, init_client_buffer, strlen(init_client_buffer), 0) < 0) {
                    perror("Erreur lors de l'envoi du message au client");
                    exit(EXIT_FAILURE);
                } 
                write_in_log(check_ls, "send", 0, client_number, init_client_buffer);
            }
        }
        else{
            strcpy(init_client_buffer, header);
            strcat(init_client_buffer, "|incorrect format");
            if (send(client_id, init_client_buffer, strlen(init_client_buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
            write_in_log(check_ls, "send", 0, client_number, init_client_buffer);
        }
        return 1;
    }
    return 0;
}

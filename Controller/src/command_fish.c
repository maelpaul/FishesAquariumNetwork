#include "command_fish.h"

#define BUFFER_SIZE 256

int add_fish_server(char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int sock) {
    char add_verif[8];
    strncpy (add_verif, buffer, 7);
    add_verif[7] = '\0';   /* null character manually added */

    if (!strcmp(add_verif, "addFish")) {

        char info[BUFFER_SIZE];
        memcpy(info, buffer, BUFFER_SIZE);
        char delim[] = " ";

        /* Cut the buffer to separate the contents.*/
	    char * verif = strtok(info, delim);
        (void) verif;
        char * name = strtok(NULL, delim);
        char * at = strtok(NULL, delim);
        (void) at;
        char * __coords = strtok(NULL, delim);
        char * __size = strtok(NULL, delim);
        char * _path = strtok(NULL, delim);
        char * path = strtok(_path, "\n");
        
        char * _coords = strtok(__coords, ",");
        char * _size = strtok(__size, ",");

        int coords[2] = {-1, -1};
        char * a_c = strtok(_coords, "x");
        char * b_c = strtok(NULL, "x");
        if (a_c != NULL && b_c != NULL) {
            coords[0] = atoi(a_c);
            coords[1] = atoi(b_c);
        }

        int size[2] = {-1, -1};
        char * a_s = strtok(_size, "x");
        char * b_s = strtok(NULL, "x");
        if (a_s != NULL && b_s != NULL) {
            size[0] = atoi(a_s);
            size[1] = atoi(b_s);
        }

        int val = -1;
        int check_path = 0;
        
        if (path != NULL) {
            if (!strcmp(path, "RandomWayPoint")) {
                check_path = 1;
                void (*new_path)(struct fish *, int, int) = &RandomWayPoint;
                pthread_mutex_lock(mutex);
                val = controller_add_fish(aquarium, coords, size, name, new_path);
                pthread_mutex_unlock(mutex);
            }
        }

        if (check_path == 0 && path != NULL) {
            strcpy(buffer, header);
            strcat(buffer, "|> NOK : Modèle de mobilité non supporté");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        else if (val == 0 && check_path == 1) {
            strcpy(buffer, header);
            strcat(buffer, "|> NOK : Poisson déjà existant");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        else if (val == 1 && check_path == 1) {
            strcpy(buffer, header);
            strcat(buffer, "|> OK");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }
        else {
            strcpy(buffer, header);
            strcat(buffer, "|> NOK : Syntaxe invalide");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }                 
        }
        return 1; // Success
    }
    else {
        return 0;
    }
}

int del_fish_server(char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int sock) {
    // Suppression d'un poisson
    char del_verif[8];
    strncpy (del_verif, buffer, 7);
    del_verif[7] = '\0';   /* null character manually added */

    if (!strcmp(del_verif, "delFish")) {
        // Suppression un poisson
        char info[BUFFER_SIZE];
        memcpy(info, buffer, BUFFER_SIZE);
        char delim[] = " ";

        char * verif = strtok(info, delim);
        (void) verif;
        char * _fish = strtok(NULL, delim);
        char * fish = strtok(_fish, "\n");

        pthread_mutex_lock(mutex);
        int val = controller_del_fish(aquarium, fish);
        pthread_mutex_unlock(mutex);

        if (val == 0) {
            strcpy(buffer, header);
            strcat(buffer, "|> NOK : Poisson inexistant");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        else if (val == 1) {
            strcpy(buffer, header);
            strcat(buffer, "|> OK");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }
        else {
            strcpy(buffer, header);
            strcat(buffer, "|> NOK : Nom de poisson invalide");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }                 
        }
        return 1;
    }
    else {
        return 0;
    }
}

int start_fish_server(char * header, char * buffer, struct aquarium * aquarium, pthread_mutex_t * mutex, int sock) {
    // Démarrage d'un poisson
    char start_verif[10];
    strncpy (start_verif, buffer, 9);
    start_verif[9] = '\0';   /* null character manually added */

    if (!strcmp(start_verif, "startFish")) {
        // Démarrage d'un poisson
        char info[BUFFER_SIZE];
        memcpy(info, buffer, BUFFER_SIZE);
        char delim[] = " ";

        /* Cut the buffer to separate the contents.*/
        char * verif = strtok(info, delim);
        (void) verif;
        char * _name = strtok(NULL, delim);
        char * name = strtok(_name, "\n");

        pthread_mutex_lock(mutex);
        int val = controller_start_fish(aquarium, name, REFRESH_TIME);
        pthread_mutex_unlock(mutex);

        if (val == 0) {
            strcpy(buffer, header);
            strcat(buffer, "|> NOK : Poisson inexistant");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }
        }
        else if (val == 1) {
            strcpy(buffer, header);
            strcat(buffer, "|> OK");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            } 
        }
        else if (val == 2) {
            strcpy(buffer, header);
            strcat(buffer, "|> NOK : Poisson déjà démaré");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }                 
        }
        else {
            strcpy(buffer, header);
            strcat(buffer, "|> NOK : Nom de poisson invalide");
            if (send(sock, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi du message au client");
                exit(EXIT_FAILURE);
            }                 
        }
        return 1;
    }
    else {
        return 0;
    }
}
        

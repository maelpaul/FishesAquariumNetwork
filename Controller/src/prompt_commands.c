#include "prompt_commands.h"

#define SEND_SIZE 256
#define F_PATH_SIZE 100

char to_send[SEND_SIZE];

void clear(char* str, int size){
    memset(str, '\0', size);
}

void prompt_load(int check_log, struct command * command, struct aquarium * aquarium, int * aquarium_flag, pthread_mutex_t * mutex_aquarium, pthread_mutex_t * mutex_aquarium_flag){
    pthread_mutex_lock(mutex_aquarium_flag);
    if (*aquarium_flag ==0) {
        pthread_mutex_unlock(mutex_aquarium_flag);
        char * aquarium_name = command->params[0];

        DIR *dir = NULL;
        struct dirent *ent = NULL;

        const char *path = "../Controller/aquariums";

        // Ouvrir le répertoire
        dir = opendir(path);
        if (dir == NULL) {
            printf("Erreur lors de l'ouverture du répertoire.\n");
            exit(EXIT_FAILURE);
        }

        int check = 0;

        // Parcourir les fichiers du répertoire
        while ((ent = readdir(dir)) != NULL) {
            // Ignorer les entrées spéciales "." et ".."
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                char f_name_cp[64];
                for (int i = 0; i < (int) strlen(ent->d_name); ++i) {
                    f_name_cp[i] = ent->d_name[i];
                }
                f_name_cp[strlen(ent->d_name)] = '\0';
                char * file_name;
                file_name = strtok(f_name_cp, ".");
                
                char f_path[F_PATH_SIZE];
                memset(f_path, 0, F_PATH_SIZE);
                
                char * file_path = "../Controller/aquariums/";
                strcat(f_path, file_path);
                strcat(f_path, ent->d_name);

                if (!strcmp(aquarium_name, file_name)) {
                    check = 1;
                    clear(to_send,SEND_SIZE);
                    strcpy(to_send, "> OK : aquarium loaded (");
                    pthread_mutex_lock(mutex_aquarium);
                    load_initial_aquarium_config(f_path, aquarium, aquarium_name);
                    int nb_views = aquarium->views_len;
                    pthread_mutex_unlock(mutex_aquarium);
                    char len[10];
                    sprintf(len, "%d ", nb_views);
                    strcat(to_send, len);

                    if (nb_views == 0 || nb_views == 1) {
                        strcat(to_send, "display view)!");
                    }
                    else {
                        strcat(to_send, "display views)!");    
                    }

                    pthread_mutex_lock(mutex_aquarium_flag);
                    *aquarium_flag=1;
                    pthread_mutex_unlock(mutex_aquarium_flag);

                    printf("%s\n",to_send);
                    write_in_log(check_log, "prompt_send", 0, 0, to_send);
                }
            }
        }

        // Fermer le répertoire
        closedir(dir);
        if (check == 0) {
            printf("> NOK : aquarium not existing\n");
            write_in_log(check_log, "print", 0, 0, "> NOK : aquarium not existing\n");
        }
    }
    else if(*aquarium_flag == 1){
        pthread_mutex_unlock(mutex_aquarium_flag);
        printf("> NOK : aquarium already loaded\n");
        write_in_log(check_log, "print", 0, 0, "> NOK : aquarium already loaded\n");
    }
}

void prompt_add_view(int check_log, struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium){
    char * view_name = command->params[0];

    char buff[SEND_SIZE];
    strcpy(buff,command->params[1]);
    int coords[2];
    coords[0]=atoi(strtok(buff,"x"));
    coords[1]=atoi(strtok(NULL,"+"));
    int size[2];
    size[0]=atoi(strtok(NULL,"+"));
    size[1]=atoi(strtok(NULL,"\0"));

    clear(buff,SEND_SIZE);

    pthread_mutex_lock(mutex_aquarium);
    if(view_name_check(aquarium, view_name)){
        pthread_mutex_unlock(mutex_aquarium);
        printf("> NOK : View already exists\n");
        write_in_log(check_log, "print", 0, 0, "> NOK : View already exists\n");
    }
    else{
        pthread_mutex_unlock(mutex_aquarium);
        clear(to_send,SEND_SIZE);
        pthread_mutex_lock(mutex_aquarium);
        add_view(aquarium, coords, size, view_name);
        pthread_mutex_unlock(mutex_aquarium);
        
        char to_send[SEND_SIZE] = "> OK : view ";
        strcat(to_send, view_name);
        strcat(to_send, " added.");
        printf("%s\n",to_send);
        write_in_log(check_log, "prompt_send", 0, 0, to_send);
    }
}

void prompt_del_view(int check_log, struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium){
    char * view_name = command->params[0];
    int removed = 0;
    pthread_mutex_lock(mutex_aquarium);
    int nb_views = aquarium->views_len;
    for(int i=0; i < nb_views; i++){
        if(removed == 0 && strcmp(aquarium->views[i]->name,view_name)==0){
            if (is_view_free(aquarium->views[i])) {
                clear(to_send,SEND_SIZE);
                del_view(aquarium, view_name);
                removed = 1;
                strcpy(to_send,"> OK : view ");
                strcat(to_send, view_name);
                strcat(to_send, " deleted.");
                printf("%s\n",to_send);
                write_in_log(check_log, "prompt_send", 0, 0, to_send);
            }
            else {
                printf("> NOK : view used by a client\n");
                write_in_log(check_log, "print", 0, 0, "> NOK : view used by a client\n");
                pthread_mutex_unlock(mutex_aquarium);
                return;
            }
        }
    }
    pthread_mutex_unlock(mutex_aquarium);
    if(!removed){
        printf("> NOK : view not existing\n");
        write_in_log(check_log, "print", 0, 0, "> NOK : view not existing\n");
    }
}

void prompt_show(int check_log, struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium){
    char * aquarium_name = command->params[0];

    if (!strcmp(aquarium_name, "aquarium")) {
        clear(to_send,SEND_SIZE);
        strcpy(to_send,"> OK : \n");
        pthread_mutex_lock(mutex_aquarium);
        controller_aquarium_print(aquarium, to_send);
        pthread_mutex_unlock(mutex_aquarium);
        printf("%s\n",to_send);
        write_in_log(check_log, "prompt_send", 0, 0, to_send);
    }
    else {
        printf("> NOK : aquarium not existing\n");
        write_in_log(check_log, "print", 0, 0, "> NOK : aquarium not existing\n");
    }
}

void prompt_save(int check_log, struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium){
    char * aquarium_name = command->params[0];

    pthread_mutex_lock(mutex_aquarium);
    save_aquarium(aquarium, aquarium_name);
    pthread_mutex_unlock(mutex_aquarium);
    printf("> OK : aquarium saved\n");
    write_in_log(check_log, "print", 0, 0, "> OK : aquarium saved\n");
}
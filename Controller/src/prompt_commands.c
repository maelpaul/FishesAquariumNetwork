#include "prompt_commands.h"

#define SEND_SIZE 256

char to_send[SEND_SIZE];

void clear(char* str, int size){
    memset(str, '\0', size);
}

void prompt_load(struct command * command, struct aquarium * aquarium, int * aquarium_flag, pthread_mutex_t * mutex_aquarium, pthread_mutex_t * mutex_aquarium_flag){
    pthread_mutex_lock(mutex_aquarium_flag);
    if (*aquarium_flag ==0) {
        pthread_mutex_unlock(mutex_aquarium_flag);
        char * aquarium_name = command->params[0];

        if (!strcmp(aquarium_name, "aquarium")) {
            clear(to_send,SEND_SIZE);
            strcpy(to_send, "> OK : aquarium loaded (");
            pthread_mutex_lock(mutex_aquarium);
            load_initial_aquarium_config("aquarium_example.txt", aquarium);
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
        }
        else {
            printf("> NOK : aquarium not existing\n");
        }
    }
    else if(*aquarium_flag == 1){
        pthread_mutex_unlock(mutex_aquarium_flag);
        printf("> NOK : aquarium already loaded\n");
    }
}

void prompt_add_view(struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium){
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
    }
    else{
        pthread_mutex_unlock(mutex_aquarium);
        clear(to_send,SEND_SIZE);
        pthread_mutex_lock(mutex_aquarium);
        add_view(aquarium, coords, size, view_name);
        pthread_mutex_unlock(mutex_aquarium);
        char to_send[256] = "> OK : view ";
        strcat(to_send, view_name);
        strcat(to_send, " added.");
        printf("%s\n",to_send);
    }
}

void prompt_del_view(struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium){
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
            }
            else {
                printf("> NOK : view used by a client\n");
                pthread_mutex_unlock(mutex_aquarium);
                return;
            }
        }
    }
    pthread_mutex_unlock(mutex_aquarium);
    if(!removed){
        printf("> NOK : view not existing\n");
    }
}

void prompt_show(struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium){
    char * aquarium_name = command->params[0];

    if (!strcmp(aquarium_name, "aquarium")) {
        clear(to_send,SEND_SIZE);
        strcpy(to_send,"> OK : \n");
        pthread_mutex_lock(mutex_aquarium);
        controller_aquarium_print(aquarium, to_send);
        pthread_mutex_unlock(mutex_aquarium);
        printf("%s\n",to_send);
    }
    else {
        printf("> NOK : aquarium not existing\n");
    }
}

void prompt_save(struct command * command, struct aquarium * aquarium, pthread_mutex_t * mutex_aquarium){
    char * aquarium_name = command->params[0];

    if (!strcmp(aquarium_name, "aquarium")) {
        pthread_mutex_lock(mutex_aquarium);
        save_aquarium(aquarium);
        pthread_mutex_unlock(mutex_aquarium);
        printf("> OK : aquarium saved\n");
    }
    else {
        printf("> NOK : aquarium not existing\n");
    }
}
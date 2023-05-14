#include "prompt_commands.h"

#define SEND_SIZE 256

char to_send[SEND_SIZE];

void clear(char* str, int size){
    memset(str, '\0', size);
}

void prompt_load(struct command * command, struct aquarium * aquarium, int * aquarium_flag, pthread_mutex_t * mutex_aquarium){
    if (*aquarium_flag ==0) {
        char * aquarium_name = command->params[0];

        if (!strcmp(aquarium_name, "aquarium")) {
            clear(to_send,SEND_SIZE);
            strcpy(to_send, "> OK : aquarium loaded (");
            pthread_mutex_lock(mutex_aquarium);
            load_initial_aquarium_config("aquarium_example.txt", aquarium);
            pthread_mutex_unlock(mutex_aquarium);
            int nb_views = aquarium->views_len;
            char len[10];
            sprintf(len, "%d ", nb_views);
            strcat(to_send, len);

            if (nb_views == 0 || nb_views == 1) {
                strcat(to_send, "display view)!");
            }
            else {
                strcat(to_send, "display views)!");    
            }

            *aquarium_flag=1;

            printf("%s\n",to_send);
        }
        else {
            printf("> NOK : aquarium not existing\n");
        }
    }
    else if(*aquarium_flag == 1){
        printf("> NOK : aquarium already loaded\n");
    }
}

void prompt_add_view(struct command * command, struct aquarium * aquarium){
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

    if(view_name_check(aquarium, view_name)){
        printf("> NOK : View already exists\n");
    }
    else{
        clear(to_send,SEND_SIZE);
        add_view(aquarium, coords, size, view_name);
        char to_send[256] = "> OK : view ";
        strcat(to_send, view_name);
        strcat(to_send, " added.");
        printf("%s\n",to_send);
    }
}

void prompt_del_view(struct command * command, struct aquarium * aquarium){
    char * view_name = command->params[0];
    int removed = 0;
    for(int i=0; i<aquarium->views_len; i++){
        if(strcmp(aquarium->views[i]->name,view_name)==0){
            clear(to_send,SEND_SIZE);
            del_view(aquarium, view_name);
            removed = 1;
            strcpy(to_send,"> OK : view ");
            strcat(to_send, view_name);
            strcat(to_send, " deleted.");
            printf("%s\n",to_send);
        }
    }
    if(!removed){
        printf("> NOK : view not existing\n");
    }
}

void prompt_show(struct command * command, struct aquarium * aquarium){
    char * aquarium_name = command->params[0];

    if (!strcmp(aquarium_name, "aquarium")) {
        clear(to_send,SEND_SIZE);
        strcpy(to_send,"> OK : \n");
        controller_aquarium_print(aquarium, to_send);
        printf("%s\n",to_send);
    }
    else {
        printf("> NOK : aquarium not existing\n");
    }
}

void prompt_save(struct command * command, struct aquarium * aquarium){
    char * aquarium_name = command->params[0];

    if (!strcmp(aquarium_name, "aquarium")) {
        save_aquarium(aquarium);
        printf("> OK : aquarium saved\n");
    }
    else {
        printf("> NOK : aquarium not existing\n");
    }
}
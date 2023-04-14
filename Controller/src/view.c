#include "view.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void view_init(struct view * view){
    for(int i=0; i<2; i++){
        view->coords[i]=0;
        view->size[i]=0;
    }
    view->name=NULL;
}

void view_create(struct view * view, int * coords, int * size, char * name, int aquarium_width, int aquarium_height){
    for(int i=0; i<2; i++){
        view->coords[i]=coords[i];
        view->size[i]=size[i];
    }
    view->name=malloc(strlen(name)+1);
    strcpy(view->name,name);
    if (view->size[0] > aquarium_width || view->size[1] > aquarium_height){
        printf("View oversized\n");
        view_init(view);
    }
}

void view_print(struct view * view){
    printf("============ View ============\n");
    printf("view name   : %s\n", view->name);
    printf("view coords : (%d,%d)\n", view->coords[0],view->coords[1]);
    printf("view size   : (%d,%d)\n", view->size[0],view->size[1]);
    printf("==============================\n");
}
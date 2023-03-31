#include "view.h"

#include <stdlib.h>

void view_init(struct view * view){
    for(int i=0; i<2; i++){
        view->coords[i]=0;
        view->size[i]=0;
    }
    view->name=NULL;
}

void view_create(struct view * view, int * coords, int * size, char * name){
    for(int i=0; i<2; i++){
        view->coords[i]=coords[i];
        view->size[i]=size[i];
    }
    view->name=name;
}
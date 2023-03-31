#include <stdio.h>
#include <stdlib.h>
#include "fish.h"

void fish_init(struct fish * fish) {
    fish->name = NULL;
    fish->path = NULL;
    for (int i = 0; i < 2; ++i) {
        fish->coords[i] = 0;
        fish->size[i] = 0;
    } 

}

void fish_create(struct fish * fish, int coords[2], int size[2], char * name, void (*path)(struct fish *)) {

}
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

void fish_create(struct fish * fish, int * coords, int * size, char * name, void (*path)(struct fish *)) {
    fish->name = name;
    fish->path = path;
    for (int i = 0; i < 2; ++i) {
        fish->coords[i] = coords[i];
        fish->size[i] = size[i];
    } 
}

void fish_print(struct fish * fish) {
    printf("============ Debug ============\n");
    printf("fish name   : %s\n", fish->name);
    printf("fish path   : %p\n", fish->path);
    printf("fish coords : (%d,%d)\n", fish->coords[0],fish->coords[1]);
    printf("fish size   : (%d,%d)\n", fish->size[0],fish->size[1]);
    printf("===============================\n");
}

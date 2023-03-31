#include <stdio.h>
#include <stdlib.h>
#include "fish.h"

void fish_init(struct fish * fish) {
    fish->name = NULL;
    fish->path = NULL;
    for (int i = 0; i < 2; ++i) {
        fish->coords[i] = 0;
        fish->size[i] = 0;
        fish->dest[i] = 0;
    }
}

void fish_create(struct fish * fish, int * coords, int * size, char * name, int * dest, void (*path)(struct fish *)) {
    fish->name = name;
    fish->path = path;
    for (int i = 0; i < 2; ++i) {
        fish->coords[i] = coords[i];
        fish->size[i] = size[i];
        fish->dest[i] = dest[i];
    } 
}

void fish_print(struct fish * fish) {
    printf("name: %s\n", fish->name);
    printf("path: %p\n", fish->path);
    for (int i = 0; i < 2; ++i) {
        printf("coords[%d]: %d\n", i, fish->coords[i]);
        printf("size[%d]: %d\n", i, fish->size[i]);
    } 
}

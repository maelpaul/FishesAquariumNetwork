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
    printf("%s\n", fish->name);
    printf("%p\n", fish->path);
    for (int i = 0; i < 2; ++i) {
        printf("%d\n", fish->coords[i]);
        printf("%d\n", fish->size[i]);
    } 
}

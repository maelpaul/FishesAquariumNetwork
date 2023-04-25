#include "fish.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fish_init(struct fish * fish) {
    fish->name = NULL;
    fish->path = NULL;
    for (int i = 0; i < 2; ++i) {
        fish->coords[i] = 0;
        fish->size[i] = 0;
        fish->dest[i] = 0;
    }
}

void fish_create(struct fish * fish, int * coords, int * size, char * name, void (*path)(struct fish *, int, int), int aquarium_width, int aquarium_height) {
    fish->name=malloc(strlen(name)+1);
    strcpy(fish->name,name);
    fish->path = path;
    (*(fish->path))(fish, aquarium_width, aquarium_height);
    for (int i = 0; i < 2; ++i) {
        fish->coords[i] = coords[i];
        fish->size[i] = size[i];
    } 
}

void fish_free(struct fish * fish) {
    free(fish->name);
    free(fish);
}

void fish_print(struct fish * fish) {
    printf("============ Fish ============\n");
    printf("fish name   : %s\n", fish->name);
    printf("fish path   : %p\n", fish->path);
    printf("fish coords : (%d,%d)\n", fish->coords[0],fish->coords[1]);
    printf("fish size   : (%d,%d)\n", fish->size[0],fish->size[1]);
    printf("fish dest   : (%d,%d)\n", fish->dest[0],fish->dest[1]);
    printf("==============================\n");
}

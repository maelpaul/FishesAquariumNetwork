#include "fish.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void fish_init(struct fish * fish) {
    fish->name = NULL;
    fish->path = NULL;
    for (int i = 0; i < 2; ++i) {
        fish->coords[i] = 0;
        fish->size[i] = 0;
        fish->dest[i] = 0;
    }
    fish->started = 0;
    fish->started_time = 0;
    fish->time_to_dest = 0;
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
    fish->started = 0;
    fish->started_time = 0;
    fish->time_to_dest = 0; 
}

void fish_free(struct fish * fish) {
    free(fish->name);
    free(fish);
}

void fish_print(struct fish * fish) {
    printf("============ Fish ============\n");
    printf("fish name    : %s\n", fish->name);
    printf("fish path    : %p\n", fish->path);
    printf("fish coords  : (%d,%d)\n", fish->coords[0],fish->coords[1]);
    printf("fish size    : (%d,%d)\n", fish->size[0],fish->size[1]);
    printf("fish dest    : (%d,%d)\n", fish->dest[0],fish->dest[1]);
    printf("fish started : %s\n", fish->started == 1 ? "True" : "False");
    printf("started time : %ld\n", fish->started_time);
    printf("time to dest : %d\n", fish->time_to_dest);
    printf("==============================\n");
}

void fish_start(struct fish * fish, int time_to_dest) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t command_time = tv.tv_sec;
    fish->started = 1;
    fish->started_time = command_time;
    fish->time_to_dest = time_to_dest;
}

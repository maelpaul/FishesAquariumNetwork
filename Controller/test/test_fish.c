#include <stdio.h>
#include <stdlib.h>

#include "fish.h"

void RandomWayPoint(struct fish * fish, int width, int height) {
    fish->dest[0] = rand() % width;
    fish->dest[1] = rand() % height;    
}

int main(){
    struct fish * fish = malloc(sizeof(struct fish));

    void (*path)(struct fish *, int, int) = &RandomWayPoint;

    fish_init(fish);

    int size[2] = {10, 5};
    int coords[2] = {200, 200};

    fish_create(fish, coords, size, "Nathan", path, 1000, 1000);
    fish_print(fish);

    free(fish);

    return 0;
}
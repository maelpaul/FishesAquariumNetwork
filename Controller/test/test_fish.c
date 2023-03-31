#include <stdio.h>
#include <stdlib.h>

#include "../src/fish.h"

void RandomWayPoint(struct fish * fish) {
    fish->dest[0] = rand() % 100;
    fish->dest[1] = rand() % 100;    
}

int main(){
    struct fish * fish = malloc(sizeof(struct fish));

    void (*path)(struct fish *) = &RandomWayPoint;

    fish_init(fish);

    int size[2] = {10, 5};
    int coords[2] = {200, 200};

    fish_create(fish, coords, size, "Nathan", path);
    fish_print(fish);

    free(fish);

    return 0;
}
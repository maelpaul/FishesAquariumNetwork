#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fish.h"

int main(){
    struct fish * fish = malloc(sizeof(struct fish));

    fish_init(fish);

    int size[2] = {10, 5};
    int coords[2] = {200, 200};
    int dest[2] = {50, 50};

    fish_create(fish, coords, size, "Nathan", dest, NULL);
    fish_print(fish);

    free(fish);

    return 0;
}
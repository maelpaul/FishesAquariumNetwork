#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/fish.h"
#include "../src/view.h"
#include "../src/aquarium.h"

int main(){
    struct aquarium * aquarium = malloc(sizeof(struct aquarium));

    void (*path)(struct fish *) = &RandomWayPoint;

    struct fish * fish1 = malloc(sizeof(struct fish));
    fish_init(fish1);
    int size1[2] = {10, 5};
    int coords1[2] = {200, 200};
    fish_create(fish1, coords1, size1, "Nathan", path);

    struct fish * fish2 = malloc(sizeof(struct fish));
    fish_init(fish2);
    int size2[2] = {10, 5};
    int coords2[2] = {200, 200};
    fish_create(fish2, coords2, size2, "Victor", path);

    struct view * view1 = malloc(sizeof(struct view));
    view_init(view1);
    int size3[2] = {400,400};
    int coords3[2] = {400,400}; 
    view_create(view1, size3, coords3, "View1");

    struct view * view2 = malloc(sizeof(struct view));
    view_init(view2);
    int size4[2] = {400,400};
    int coords4[2] = {400,400}; 
    view_create(view2, size4, coords4, "View2");

    aquarium_init(aquarium);
    int size5[2] = {1000,1000};
    aquarium_create(aquarium, size5, "Aquarium");
    add_fish(aquarium, fish1);
    add_fish(aquarium, fish2);
    add_view(aquarium, view1);
    add_view(aquarium, view2);
    aquarium_print(aquarium);
    del_fish(aquarium, "Nathan");
    del_view(aquarium, "View2");
    aquarium_print(aquarium);
    for (int i = 0; i < 15; i++) {
        add_fish(aquarium, fish2);
        add_view(aquarium, view1);
    }
    aquarium_print(aquarium);
    aquarium_free(aquarium);

    return 0; 
}

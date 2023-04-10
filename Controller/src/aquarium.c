#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aquarium.h"
#include "fish.h"
#include "view.h"

void aquarium_init(struct aquarium * aquarium) {
    aquarium->name = NULL;
    aquarium->fishes = NULL;
    aquarium->views = NULL;
    aquarium->fishes_len = 0;
    aquarium->views_len = 0;
    for (int i = 0; i < 2; ++i) {
        aquarium->size[i] = 0;
    } 
}

void aquarium_create(struct aquarium * aquarium, int * size, char * name) {
    aquarium->name = name;
    aquarium->fishes = malloc(DEFAULT_NUMBER_FISH * sizeof(struct fish *));
    aquarium->views = malloc(DEFAULT_NUMBER_VIEW * sizeof(struct view *));
    aquarium->fishes_len = 0;
    aquarium->views_len = 0;
    for (int i = 0; i < 2; ++i) {
        aquarium->size[i] = size[i];
    } 
}

int fish_name_check(struct aquarium * aquarium, char * fish_name) {
    for (int i = 0; i < aquarium->fishes_len; i++) {
        if (strcmp(aquarium->fishes[i]->name, fish_name) == 0) {
            return 1;
        }
    }
    return 0;
}

int view_name_check(struct aquarium * aquarium, char * view_name) {
    for (int i = 0; i < aquarium->views_len; i++) {
        if (strcmp(aquarium->views[i]->name, view_name) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_fish(struct aquarium * aquarium, int * coords, int * size, char * name, void (*path)(struct fish *, int, int)) {
    int check = fish_name_check(aquarium, name);
    if (check == 0) {
        if (aquarium->fishes_len < DEFAULT_NUMBER_FISH) {
            struct fish * fish = malloc(sizeof(struct fish));
            fish_init(fish);
            fish_create(fish, coords, size, name, path, aquarium->size[0], aquarium->size[1]);
            aquarium->fishes[aquarium->fishes_len] = fish;
            aquarium->fishes_len++;
        }
        else {
            aquarium->fishes = realloc(aquarium->fishes, (aquarium->fishes_len+1) * sizeof(struct fish *));
            struct fish * fish = malloc(sizeof(struct fish));
            fish_init(fish);
            fish_create(fish, coords, size, name, path, aquarium->size[0], aquarium->size[1]);
            aquarium->fishes[aquarium->fishes_len] = fish;
            aquarium->fishes_len++;
        }
    }
}

void add_view(struct aquarium * aquarium, int * coords, int * size, char * name) {
    int check = view_name_check(aquarium, name);
    if (check == 0) {
        if (aquarium->views_len < DEFAULT_NUMBER_VIEW) {
            struct view * view = malloc(sizeof(struct view));
            view_init(view);
            view_create(view, coords, size, name, aquarium->size[0], aquarium->size[1]);
            aquarium->views[aquarium->views_len] = view;
            aquarium->views_len++;
        }
        else {
            aquarium->views = realloc(aquarium->views, (aquarium->views_len+1) * sizeof(struct view *));
            struct view * view = malloc(sizeof(struct view));
            view_init(view);
            view_create(view, coords, size, name, aquarium->size[0], aquarium->size[1]);
            aquarium->views[aquarium->views_len] = view;
            aquarium->views_len++;
        }
    }
}

void del_fish(struct aquarium * aquarium, char * fish_name) {
    for (int i = 0; i < aquarium->fishes_len; i++) {
        if (strcmp(aquarium->fishes[i]->name, fish_name) == 0) {
            free(aquarium->fishes[i]);
            aquarium->fishes[i] = NULL;
            for (int j = i; j < aquarium->fishes_len-1; j++) {
                aquarium->fishes[j] = aquarium->fishes[j+1];
            }
            aquarium->fishes[aquarium->fishes_len-1] = NULL;
            aquarium->fishes_len--;
        }
    }
}

void del_view(struct aquarium * aquarium, char * view_name) {
    for (int i = 0; i < aquarium->views_len; i++) {
        if (strcmp(aquarium->views[i]->name, view_name) == 0) {
            free(aquarium->views[i]);
            aquarium->views[i] = NULL;
            for (int j = i; j < aquarium->views_len-1; j++) {
                aquarium->views[j] = aquarium->views[j+1];
            }
            aquarium->views[aquarium->views_len-1] = NULL;
            aquarium->views_len--;
        }
    }
}

void aquarium_free(struct aquarium * aquarium) {
    for (int i = 0; i < aquarium->fishes_len; i++) {
        free(aquarium->fishes[i]);
    }
    for (int i = 0; i < aquarium->views_len; i++) {
        free(aquarium->views[i]);
    }
    free(aquarium->fishes);
    free(aquarium->views);
    free(aquarium);
}

void aquarium_print(struct aquarium * aquarium) {
    printf("name: %s\n", aquarium->name);
    for (int i = 0; i < aquarium->fishes_len; i++) {
        printf("fish %d:\n", i);
        fish_print(aquarium->fishes[i]);
    }
    for (int i = 0; i < aquarium->views_len; i++) {
        printf("view %d:\n", i);
        view_print(aquarium->views[i]);
    }
    for (int i = 0; i < 2; ++i) {
        printf("size[%d]: %d\n", i, aquarium->size[i]);
    }
    printf("nb_fishes: %d\n", aquarium->fishes_len); 
    printf("nb_views: %d\n", aquarium->views_len);
}

int get_aquarium_width(struct aquarium * aquarium) {
    return aquarium->size[0];
}

int get_aquarium_height(struct aquarium * aquarium) {
    return aquarium->size[1];
}

void RandomWayPoint(struct fish * fish, int width, int height) {
    fish->dest[0] = rand() % width;
    fish->dest[1] = rand() % height;    
}

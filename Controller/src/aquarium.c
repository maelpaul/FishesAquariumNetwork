#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
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

void aquarium_create(struct aquarium * aquarium, int * size, const char * name) {
    aquarium->name = malloc(strlen(name)+1);
    strcpy(aquarium->name, name);
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

int add_fish(struct aquarium * aquarium, int * coords, int * size, char * name, void (*path)(struct fish *, int, int)) {
    int val = aquarium->fishes_len;
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
    return aquarium->fishes_len - val;
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

int del_fish(struct aquarium * aquarium, char * fish_name) {
    int val = aquarium->fishes_len;
    for (int i = 0; i < aquarium->fishes_len; i++) {
        if (strcmp(aquarium->fishes[i]->name, fish_name) == 0) {
            fish_free(aquarium->fishes[i]);
            aquarium->fishes[i] = NULL;
            for (int j = i; j < aquarium->fishes_len-1; j++) {
                aquarium->fishes[j] = aquarium->fishes[j+1];
            }
            aquarium->fishes[aquarium->fishes_len-1] = NULL;
            aquarium->fishes_len--;
        }
    }
    return val - aquarium->fishes_len;
}

void del_view(struct aquarium * aquarium, char * view_name) {
    for (int i = 0; i < aquarium->views_len; i++) {
        if (strcmp(aquarium->views[i]->name, view_name) == 0) {
            view_free(aquarium->views[i]);
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
    free(aquarium->name);
    for (int i = 0; i < aquarium->fishes_len; i++) {
        fish_free(aquarium->fishes[i]);
    }
    for (int i = 0; i < aquarium->views_len; i++) {
        view_free(aquarium->views[i]);
    }
    free(aquarium->fishes);
    free(aquarium->views);
    free(aquarium);
}

void aquarium_print(struct aquarium * aquarium) {
    printf("------------------------------\n");
    printf("aquarium name: %s\n", aquarium->name);
    printf("aquarium size (width, height): (%d,%d)\n",aquarium->size[0],aquarium->size[1]);
    printf("nb_fishes: %d\n", aquarium->fishes_len); 
    printf("nb_views: %d\n", aquarium->views_len);
    for (int i = 0; i < aquarium->fishes_len; i++) {
        printf("fish %d:\n", i);
        fish_print(aquarium->fishes[i]);
    }
    for (int i = 0; i < aquarium->views_len; i++) {
        printf("view %d:\n", i);
        view_print(aquarium->views[i]);
    }
    printf("------------------------------\n");
}

int get_aquarium_width(struct aquarium * aquarium) {
    return aquarium->size[0];
}

int get_aquarium_height(struct aquarium * aquarium) {
    return aquarium->size[1];
}

void RandomWayPoint(struct fish * fish, int width, int height) {
    srand( time( NULL ) );
    int aquarium_size[2] = {width, height};
    fish->dest[0] = rand() % (width - fish->size[0]);
    fish->dest[1] = rand() % (height - fish->size[1]);
    for (int i = 0; i < 2; ++i) {
        assert(fish->dest[i] > -1);
        assert(fish->dest[i] < aquarium_size[i]);
    }    
}

void HorizontalWayPoint(struct fish * fish, int width, int height) {
    srand( time( NULL ) );
    int aquarium_size[2] = {width, height};
    (void) height;
    fish->dest[0] = rand() % (width - fish->size[0]);
    fish->dest[1] = fish->coords[1];
    for (int i = 0; i < 2; ++i) {
        assert(fish->dest[i] > -1);
        assert(fish->dest[i] < aquarium_size[i]);
    }   
}

void VerticalWayPoint(struct fish * fish, int width, int height) {
    srand( time( NULL ) );
    int aquarium_size[2] = {width, height};
    (void) width;
    fish->dest[0] = fish->coords[0];
    fish->dest[1] = rand() % (height - fish->size[1]); 
    for (int i = 0; i < 2; ++i) {
        assert(fish->dest[i] > -1);
        assert(fish->dest[i] < aquarium_size[i]);
    }   
}

int start_fish(struct aquarium * aquarium, char * fish_name, int time_to_dest) {
    int val = 0;
    for (int i = 0; i < aquarium->fishes_len; i++) {
        if (strcmp(aquarium->fishes[i]->name, fish_name) == 0) {
            if (aquarium->fishes[i]->started == 0) {
                val = 1;
                (*(aquarium->fishes[i]->path))(aquarium->fishes[i], aquarium->size[0], aquarium->size[1]);
                fish_start(aquarium->fishes[i], time_to_dest);
            }
            else {
                val = 2;
            }
        }
    }
    return val;
}

void update_fishes(struct aquarium * aquarium, int refresh_time) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t command_time = tv.tv_sec;
    for (int i = 0; i < aquarium->fishes_len; i++) {
        struct fish * fish = aquarium->fishes[i];
        if (fish->started != 0) {
            int new_time_to_dest = fish->time_to_dest - (command_time - fish->started_time);
            if (new_time_to_dest <= 0) {
                fish->coords[0] = fish->dest[0];
                fish->coords[1] = fish->dest[1];
                fish->path(fish, get_aquarium_width(aquarium), get_aquarium_height(aquarium));
                fish->time_to_dest = refresh_time;
                fish->started_time = command_time;
            }
            else {
                int x_dist = abs(fish->coords[0] - fish->dest[0]);
                int y_dist = abs(fish->coords[1] - fish->dest[1]);
                int x_done = x_dist * (fish->time_to_dest - new_time_to_dest) / fish->time_to_dest;
                int y_done = y_dist * (fish->time_to_dest - new_time_to_dest) / fish->time_to_dest;
                if (fish->coords[0] >= fish->dest[0]) {
                    fish->coords[0] = fish->coords[0] - x_done;
                }
                else {
                    fish->coords[0] = fish->coords[0] + x_done;
                }
                if (fish->coords[1] >= fish->dest[1]) {
                    fish->coords[1] = fish->coords[1] - y_done;
                }
                else {
                    fish->coords[1] = fish->coords[1] + y_done;
                }
                fish->time_to_dest = new_time_to_dest;
            }
        }
    }
}

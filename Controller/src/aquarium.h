#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

#include <time.h>

#define DEFAULT_NUMBER_FISH 10
#define DEFAULT_NUMBER_VIEW 10

struct aquarium {
    struct fish ** fishes;
    int fishes_len;
    struct view ** views;
    int views_len;
    int size[2];
    char * name;
};

void aquarium_init(struct aquarium * aquarium);

void aquarium_create(struct aquarium * aquarium, int * size, const char * name);

int fish_name_check(struct aquarium * aquarium, char * fish_name);

int view_name_check(struct aquarium * aquarium, char * view_name);

int add_fish(struct aquarium * aquarium, int * coords, int * size, char * name, void (*path)(struct fish *, int, int));

void add_view(struct aquarium * aquarium, int * coords, int * size, char * name);

int del_fish(struct aquarium * aquarium, char * fish_name);

void del_view(struct aquarium * aquarium, char * view_name);

void aquarium_free(struct aquarium * aquarium);

void aquarium_print(struct aquarium * aquarium);

int get_aquarium_width(struct aquarium * aquarium);

int get_aquarium_height(struct aquarium * aquarium);

void RandomWayPoint(struct fish * fish, int width, int height);

void HorizontalWayPoint(struct fish * fish, int width, int height);

void VerticalWayPoint(struct fish * fish, int width, int height);

int start_fish(struct aquarium * aquarium, char * fish_name, int time_to_dest);

void update_fishes(struct aquarium * aquarium, int refresh_time);

#endif
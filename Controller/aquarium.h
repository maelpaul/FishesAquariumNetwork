#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

struct aquarium {
    struct fish ** fishes;
    int fishes_len;
    struct view ** views;
    int views_len;
    int size[2];
    char * name;
};

void aquarium_init(struct aquarium * aquarium);

void aquarium_create(struct aquarium * aquarium, int * size, char * name);

void add_fish(struct aquarium * aquarium, struct fish * fish);

void add_view(struct aquarium * aquarium, struct view * view);

void del_fish(struct aquarium * aquarium, char * fish_name);

void del_view(struct aquarium * aquarium, char * view_name);

void aquarium_free(struct aquarium * aquarium);

void aquarium_print(struct aquarium * aquarium);

void RandomWayPoint(struct fish * fish);

#endif
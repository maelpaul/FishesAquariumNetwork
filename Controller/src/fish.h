#ifndef _FISH_H_
#define _FISH_H_

struct fish {
    int coords[2];
    int size[2];
    char * name;
    int dest[2];
    void (*path)(struct fish *, int, int);
};

void fish_init(struct fish * fish);

void fish_create(struct fish * fish, int * coords, int * size, char * name, void (*path)(struct fish *, int, int), int aquarium_width, int aquarium_height);

void fish_print(struct fish * fish);

#endif
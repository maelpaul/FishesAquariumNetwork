#ifndef _FISH_H_
#define _FISH_H_

struct fish {
    int coords[2];
    int size[2];
    char * name;
    int dest[2];
    void (*path)(struct fish *);
};

void fish_init(struct fish * fish);

void fish_create(struct fish * fish, int * coords, int * size, char * name, int * dest, void (*path)(struct fish *));

void fish_print(struct fish * fish);

#endif
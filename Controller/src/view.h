#ifndef _VIEW_H_
#define _VIEW_H_

struct view {
    int coords[2];
    int size[2];
    char * name;
};

void view_init(struct view * view);

void view_create(struct view * view, int * coords, int * size, char * name, int aquarium_width, int aquarium_height);

void view_print(struct view * view);

#endif
#ifndef _VIEW_H_
#define _VIEW_H_

struct view {
    int coords[2];
    int size[2];
    char * name;
    int is_free;
};

void view_init(struct view * view);

void view_create(struct view * view, int * coords, int * size, char * name, int aquarium_width, int aquarium_height);

int is_view_free(struct view * view);

void change_view_status(struct view * view);

void view_free(struct view * view);

void view_print(struct view * view);

#endif
#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

struct view {
    struct fish * fishes;
    int fishes_len;
    struct view * views;
    int view_len;
    int size[2];
    char * name;
};

#endif
#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_

struct view {
    struct fish * fishes;
    struct view * views;
    int size[2];
    char * name;
};

#endif
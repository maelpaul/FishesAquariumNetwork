#ifndef _FISH_H_
#define _FISH_H_

struct fish {
    int coords[2];
    int size[2];
    char * name;
    void (*path)(struct fish *);
};

#endif
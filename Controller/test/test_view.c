#include <stdlib.h>

#include "view.h"


int main(int argc, char** argv){
    (void)argc;

    struct view * view = malloc(sizeof(struct view));

    view_init(view);

    int size[2] = {400,400};
    int coords[2] = {400,400}; 

    view_create(view, size, coords, argv[1]);

    view_print(view);

    free(view);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "view.h"

int main(){
    int width = 1000;
    int height = 1000;

    struct view * view = malloc(sizeof(struct view));

    view_init(view);

    int size[2] = {1400,1400};
    int coords[2] = {400,400}; 

    view_create(view, coords, size, "View", width, height);
    
    assert(view->name == NULL);

    assert(is_view_free(view) == 1);

    view_print(view);

    int size2[2] = {400,400};

    view_create(view, coords, size2, "View", width, height);

    assert(view->name != NULL);

    assert(is_view_free(view) == 1);

    view_print(view);

    change_view_status(view);

    assert(is_view_free(view) == 0);

    view_print(view);

    view_free(view);

    return EXIT_SUCCESS;
}
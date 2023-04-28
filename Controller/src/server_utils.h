#include "aquarium.h"
#include "view.h"
#include "fish.h"

struct config {
    int controller_port;
    int display_timeout_value;
    int fish_update_interval;
};

int load_config(const char *filename, struct config *conf);

int load_initial_aquarium_config(const char *filename, struct aquarium *aquarium);

int controller_add_fish(struct aquarium *aquarium, int * coords, int * size, char * name, void (*path)(struct fish *, int, int));

int controller_del_fish(struct aquarium *aquarium, char *fish);

char * client_connection(struct aquarium *aquarium, char* view_name);

char * find_and_attibute_free_view(struct aquarium *aquarium);

int hello_command_check(char * buffer, char * view_name);

char * get_fish(struct aquarium *aquarium, char * fish_name);


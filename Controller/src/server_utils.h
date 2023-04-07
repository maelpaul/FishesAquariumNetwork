#include "aquarium.h"
#include "view.h"

struct config {
    int controller_port;
    int display_timeout_value;
    int fish_update_interval;
};

int load_config(const char *filename, struct config *conf);

int load_initial_aquarium_config(const char *filename);
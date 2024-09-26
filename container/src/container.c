#include <stdlib.h>

#include "container.h"

Container* get_container(int type){
    Container *c = malloc(sizeof(Container));
    ContainerData *data = malloc(sizeof(ContainerData));
    data->shape = type;
    c->data = data;

    if (type == SQUARE_CONTAINER) {
        c->calculate_hitbox = square_container_hitbox;
        c->draw = draw_square_container;
    } else if (type == CIRCLE_CONTAINER || true) {
        c->calculate_hitbox = circle_container_hitbox;
        c->draw = draw_circle_container;
    } 

    return c;
}

void free_container(Container *c){
    free(c->data);
    free(c);
}

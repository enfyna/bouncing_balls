#include <stdlib.h>

#include "container.h"

int min(int a, int b){
    return (a < b) ? a : b;
}

Container* get_container(int type){
    Container *c = malloc(sizeof(Container));

    if (type == SQUARE_CONTAINER) {
        c->calculate_hitbox = square_container_hitbox;
        c->draw = draw_square_container;
    } else if (type == CIRCLE_CONTAINER) {
        c->calculate_hitbox = circle_container_hitbox;
        c->draw = draw_circle_container;
    } 

    return c;
}

void free_container(Container *c){
    free(c);
    c = NULL;
}

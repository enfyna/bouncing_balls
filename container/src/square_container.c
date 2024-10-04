#include "container.h"
#include "raylib.h"

void draw_square_container(ContainerData* c){
    float x = c->center.x - (c->size / 2.0);
    float y = c->center.y - (c->size / 2.0);

    DrawRectangle(
        x - c->padding,
        y - c->padding,
        c->size + c->padding * 2.0,
        c->size + c->padding * 2.0,
        c->color
    );
    DrawRectangle(
        x, y,
        c->size, c->size,
        c->bg_color
    );
}

void square_container_hitbox(ContainerData* c, ball* b){
    const float x_diff = (c->size / 2.0);
    const float y_diff = (c->size / 2.0);

    b->speed->y += c->gravity * c->delta;

    float nx = b->pos->x + b->speed->x * c->delta; 
    float ny = b->pos->y + b->speed->y * c->delta; 

    if (nx - b->size <= c->center.x - x_diff) {
        nx = c->center.x - x_diff + b->size;
        b->speed->x *= c->damp;
    } else if (nx + b->size >= c->center.x + x_diff) {
        nx = c->center.x + x_diff - b->size;
        b->speed->x *= c->damp;
    }

    if (ny - b->size < c->center.y - y_diff) {
        ny = c->center.y - y_diff + b->size;
        b->speed->y *= c->damp;
    } else if (ny + b->size > c->center.y + y_diff) {
        ny = c->center.y + y_diff - b->size;
        b->speed->y *= c->damp;
    }

    if (c->debug) {
        DrawLine(
            b->pos->x, b->pos->y,
            b->pos->x + b->speed->x,
            b->pos->y + b->speed->y,
            WHITE
        );
    }

    b->pos->x = nx;
    b->pos->y = ny;
}

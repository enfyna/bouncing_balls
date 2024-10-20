#include "container.h"
#include "raylib.h"

static short min(short a, short b){
    return (a < b) ? a : b;
}

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
    for (short i = min(b->point_count, LINE_LEN) - 1; i >= 0; i--) {
        if (c->line_mode == 3) {
            if (i == 0) continue;
            if (b->point_count > LINE_LEN && i == b->point_count % LINE_LEN) continue;
            DrawLine(
                b->points[i].x, b->points[i].y,
                b->points[i - 1].x, b->points[i - 1].y,
                b->color
            );
            continue;
        } 
        DrawLine(
            b->pos->x, b->pos->y,
            b->points[i].x, b->points[i].y,
            b->color
        );
    }
    if (c->line_mode == 3 
        // draw only on wrap
        && LINE_LEN <= b->point_count 
        // dont draw on point change
        && b->point_count % LINE_LEN != 0
        ) {
        DrawLine(
            b->points[0].x, b->points[0].y,
            b->points[LINE_LEN - 1].x, b->points[LINE_LEN - 1].y,
            b->color
        );
    }
    const float x_diff = (c->size / 2.0);
    const float y_diff = (c->size / 2.0);

    b->speed->y += c->gravity * c->delta;

    float nx = b->pos->x + b->speed->x * c->delta; 
    float ny = b->pos->y + b->speed->y * c->delta; 

    if (c->line_mode == 2 || c->line_mode == 3) {
        b->points[b->point_count % LINE_LEN] = *b->pos;
        b->point_count += 1;
    }

    short  collision = 0;
    if (nx - b->size <= c->center.x - x_diff) {
        nx = c->center.x - x_diff + b->size;
        b->speed->x *= -c->damp;
        collision = 1;
    } else if (nx + b->size >= c->center.x + x_diff) {
        nx = c->center.x + x_diff - b->size;
        b->speed->x *= -c->damp;
        collision = 2;
    }

    if (ny - b->size < c->center.y - y_diff) {
        ny = c->center.y - y_diff + b->size;
        b->speed->y *= -c->damp;
        collision = 3;
    } else if (ny + b->size > c->center.y + y_diff) {
        ny = c->center.y + y_diff - b->size;
        b->speed->y *= -c->damp;
        collision = 4;
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

    if (c->line_mode == 1 && collision > 0) {
        if (collision == 1) {
            b->points[b->point_count % LINE_LEN] = (Vector2){ b->pos->x - b->size, ny };
        } else if (collision == 2) {
            b->points[b->point_count % LINE_LEN] = (Vector2){ b->pos->x + b->size, ny };
        } else if (collision == 3) {
            b->points[b->point_count % LINE_LEN] = (Vector2){ nx, b->pos->y - b->size };
        } else if (collision == 4) {
            b->points[b->point_count % LINE_LEN] = (Vector2){ nx, b->pos->y + b->size };
        }
        b->point_count += 1;
    }
    return;
}

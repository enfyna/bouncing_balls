#include <math.h>
#include <stdio.h>

#include "container.h"
#include "raylib.h"
#include "raymath.h"

static Vector2 rotateVec2(Vector2 size, float deg);
static void draw_degree(ContainerData* c, Color color, float deg);
static void print_debug(ContainerData* c, ball* b);
static short min(short a, short b){
    return (a < b) ? a : b;
}

void draw_circle_container(ContainerData* c){
    DrawRing(
        c->center,
        c->size / 2.0,
        c->size / 2.0 + c->padding,
        0, 360,
        64,
        c->color
    );
}

void circle_container_hitbox(ContainerData* c, ball* b){
    if (c->debug) {
        print_debug(c, b);
    }
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
    b->speed->y += c->gravity * c->delta;

    const float nx = b->pos->x + b->speed->x * c->delta; 
    const float ny = b->pos->y + b->speed->y * c->delta; 

    const float x_diff = c->center.x - nx;
    const float y_diff = c->center.y - ny;

    const float n_diff = pow(fabs(x_diff), 2) + pow(fabs(y_diff), 2);
    const float c_c2 = pow((c->size - b->size * 2) / 2.0, 2);

    if (c->line_mode == 2 || c->line_mode == 3) {
        b->points[b->point_count % LINE_LEN] = *b->pos;
        b->point_count += 1;
    }

    // is ball distance shorter than the container radius
    if (c_c2 > n_diff) {
        // there is no collision then
        b->pos->x = nx;
        b->pos->y = ny;
        return;
    }

    Vector2 normal = { x_diff, y_diff };
    normal = Vector2Normalize(normal);

    float dotProduct = Vector2DotProduct(*b->speed, normal);
    Vector2 res = {
        b->speed->x - 2 * dotProduct * normal.x,
        b->speed->y - 2 * dotProduct * normal.y
    };
    res = Vector2Scale(res, c->damp);
    b->speed->x = res.x;
    b->speed->y = res.y;

    float rad = atan(y_diff / x_diff);
    float deg = rad * RAD2DEG;
    if (x_diff > 0) {
        deg += 180;
    }
    Vector2 collision_point = rotateVec2(
        (Vector2){ c->size / 2.0, c->size / 2.0 },
        deg
    );
    collision_point.x += c->center.x;
    collision_point.y += c->center.y;

    if (c->line_mode == 1) {
        b->points[b->point_count % LINE_LEN] = collision_point;
        b->point_count += 1;
    }
    return;
}

static void print_debug(ContainerData* c, ball* b){
    b->speed->y += c->gravity * c->delta;

    const float nx = b->pos->x + b->speed->x * c->delta; 
    const float ny = b->pos->y + b->speed->y * c->delta; 

    const float x_diff = c->center.x - nx;
    const float y_diff = c->center.y - ny;

    float rad = atan(y_diff / x_diff);
    float deg = rad * RAD2DEG;
    if (x_diff > 0) {
        deg += 180;
    }

    draw_degree(c, BLUE, 0);
    draw_degree(c, BLUE, 90);
    draw_degree(c, BLUE, 120);
    draw_degree(c, RED, deg);

    DrawLine(
        b->pos->x, b->pos->y,
        b->pos->x + b->speed->x,
        b->pos->y + b->speed->y,
        WHITE
    );
}

static void draw_degree(ContainerData* c, Color color, float deg){
    Vector2 res = rotateVec2(
        (Vector2){ c->size / 2.0, c->size / 2.0 },
        deg
    );
    printf("deg: %lf\n", deg + 90);
    DrawLine(
        c->center.x,
        c->center.y,
        c->center.x + res.x,
        c->center.y + res.y,
        color
    );
}

static Vector2 rotateVec2(Vector2 size, float deg){
    float x = size.x;
    float y = size.y;

    const float mx = cos(deg * DEG2RAD);
    const float my = sin(deg * DEG2RAD);

    x *= mx;
    y *= my;

    size.x = x;
    size.y = y;

    return size;
}

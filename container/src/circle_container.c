#include <math.h>
#include <stdio.h>

#include "container.h"
#include "raylib.h"
#include "raymath.h"

static Vector2 rotateVec2(Vector2 size, float deg);
static void draw_degree(ContainerData* c, Color color, float deg);
static void print_debug(ContainerData* c, ball* b);

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
    b->speed->y += c->gravity * c->delta;

    const float nx = b->pos->x + b->speed->x * c->delta; 
    const float ny = b->pos->y + b->speed->y * c->delta; 

    const float x_diff = c->center.x - nx;
    const float y_diff = c->center.y - ny;

    const float n_diff = pow(fabs(x_diff), 2) + pow(fabs(y_diff), 2);
    const float c_c2 = pow((c->size - b->size * 2) / 2.0, 2);

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
    float speed = Vector2Length(res);
    if (speed < c->size / 2.0) {
        res = Vector2Scale(res, 0.8);
    }
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

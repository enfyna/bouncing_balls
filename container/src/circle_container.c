#include <stdlib.h>
#include <math.h>

#include "container.h"
#include "raylib.h"

void draw_circle_container(ContainerData* c, int screen_width, int screen_height){
    DrawCircle(
        screen_width, screen_height,
        (c->size + c->padding * 2) / 2.0,
        c->color
    );

    DrawCircle(screen_width, screen_height, c->size / 2.0, c->bg_color);
}

void circle_container_hitbox(ContainerData* c, Vector2 *b_pos, Vector2 *b_spd){
    float x_diff = c->center.x - b_pos->x;
    float y_diff = c->center.y - b_pos->y;

    float dist = pow(fabs(x_diff), 2) + pow(fabs(y_diff), 2);
    
    if (dist > pow((c->size / 2.0) - c->ball_size, 2)) {
        b_spd->y = -(b_spd->y + c->gravity); // cancel gravity

        if (b_pos->x > c->center.x + 50) {
            b_spd->x = -fabs(b_spd->x);
        } else if (b_pos->x < c->center.x - 50){
            b_spd->x = fabs(b_spd->x);
        }

        if(b_spd->x == 0){
            b_spd->x = random() % 10 - 5;
            b_spd->y += random() % 3;
        }
    }
}


#include <stdlib.h>

#include "container.h"
#include "raylib.h"

void draw_square_container(ContainerData* c, int screen_width, int screen_height){
    int x = c->center.x - (c->size / 2.0);
    int y = c->center.y - (c->size / 2.0);

    DrawRectangle(
        x - c->padding,
        y - c->padding,
        c->size,
        c->size,
        c->color
    );
    DrawRectangle(
        x, y,
        c->size, c->size,
        c->bg_color
    );
}

void square_container_hitbox(ContainerData* c, Vector2 *b_pos, Vector2 *b_spd){
    if(b_pos->y < 50 + c->ball_size){
        b_spd->y = +(random() % 7 + 3);
        b_pos->y = 50 + c->ball_size;
    } else if (b_pos->y > c->center.y - 50 - c->ball_size){
        b_spd->y = -(random() % 10 + 3);
        b_pos->y = c->center.y - 50 - c->ball_size;
        if (b_spd->x == 0) {
            b_spd->x = (random() % 10) - 5;
        }
    } 
    if(b_pos->x < 50 + c->ball_size){
        b_spd->x = random() % 7 + 3;
        b_pos->x = 50 + c->ball_size;
    } else if (b_pos->x > c->center.x - 50 - c->ball_size){
        b_spd->x = -(random() % 7 + 3);
        b_pos->x = c->center.x - 50 - c->ball_size;
    }
}


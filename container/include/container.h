#include "raylib.h"

#define CIRCLE_CONTAINER 0
#define SQUARE_CONTAINER 1

typedef struct {
    int size;
    int padding;
    int ball_size;
    int shape;
    double gravity;
    Vector2 center;
    Color color;
    Color bg_color;
} ContainerData;

typedef void (*hitbox_calculator)(ContainerData* c, Vector2 *ball_pos, Vector2 *ball_spd);
typedef void (*painter) (ContainerData* c, int screen_width, int screen_height);

typedef struct {
    ContainerData *data;
    hitbox_calculator calculate_hitbox;
    painter draw;
} Container;

// Base Container Functions
Container *get_container(int type);
void free_container(Container *c);

// Circle Container Functions
void draw_circle_container(ContainerData* c, int screen_width, int screen_height);
void circle_container_hitbox(ContainerData* c, Vector2 *b_pos, Vector2 *b_spd);

// Square Container Functions
void draw_square_container(ContainerData* c, int screen_width, int screen_height);
void square_container_hitbox(ContainerData* c, Vector2 *b_pos, Vector2 *b_spd);

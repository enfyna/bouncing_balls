#include "raylib.h"

#define CIRCLE_CONTAINER 0
#define SQUARE_CONTAINER 1

typedef struct {
    int id;
    Vector2* pos;
    Vector2* speed;
    Color color;
    short size;
    short padding;
    Vector2* points;
    int point_count;
    int line_len;
} ball;

typedef struct {
    int size;
    int padding;
    short shape;
    double gravity;
    float delta;
    float damp;
    Vector2 center;
    Color color;
    Color bg_color;
    bool debug;
    short line_mode;
} ContainerData;

typedef void (*hitbox_calculator)(ContainerData* c, ball* b);
typedef void (*painter) (ContainerData* c);

typedef struct {
    ContainerData data;
    hitbox_calculator calculate_hitbox;
    painter draw;
} Container;

int min(int, int);

// Base Container Functions
Container *get_container(int type);
void free_container(Container *c);

// Circle Container Functions
void draw_circle_container(ContainerData* c);
void circle_container_hitbox(ContainerData* c, ball* b);

// Square Container Functions
void draw_square_container(ContainerData* c);
void square_container_hitbox(ContainerData* c, ball* b);

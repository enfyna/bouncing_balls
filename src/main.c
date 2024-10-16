#include "raylib.h"
#include "container.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 400
#define WIDTH HEIGHT
#define CENTER (Vector2){WIDTH / 2.0, HEIGHT / 2.0}
#define BG_COLOR BLACK

#define TITLE "Bouncing Balls"
#define FPS 60.0

#define CONTAINER_SIZE 200
#define CONTAINER_PADDING 4
#define CONTAINER_WIDTH CONTAINER_SIZE + CONTAINER_PADDING * 2
#define CONTAINER_COLOR RAYWHITE

#define GRAVITY 1000 // 100.0

void draw_debug_container(); 
void draw_container(void);

ball* create_balls(ball* balls, int ball_count);
void free_balls(ball* balls, int ball_count);

int main(int argc, char **argv){
    int ball_count = 10;
    short c_shape = 0;
    bool debug = false;

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "circle")) {
            c_shape = 0;
        } else if (!strcmp(argv[i], "square")) {
            c_shape = 1;
        } else if (!strcmp(argv[i], "-bc")) {
            ball_count = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "debug")) {
            debug = true;
        }
    }

    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    Container* container = get_container(c_shape);
    ContainerData data = {
        .center = CENTER,
        .size = CONTAINER_SIZE,
        .padding = CONTAINER_PADDING,
        .shape = c_shape,
        .gravity = GRAVITY,
        .damp = -0.8,
        .bg_color = BG_COLOR,
        .color = CONTAINER_COLOR,
        .debug = debug,
    };
    container->data = &data;


    ball* balls = create_balls(NULL, ball_count);
    while (!WindowShouldClose() && !IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
            ClearBackground(BG_COLOR);
            Vector2 m_pos = GetMousePosition();

            container->data->delta = GetFrameTime();

            container->draw(container->data);
            if (debug) {
                const char* m_str = TextFormat("x: %d y: %d", (int)m_pos.x, (int)m_pos.y);
                draw_debug_container();
                DrawText(m_str, m_pos.x + 10, m_pos.y + 10, 20, WHITE);
                DrawCircle(m_pos.x, m_pos.y, 12, IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? BLUE : IsMouseButtonDown(MOUSE_RIGHT_BUTTON) ? GREEN : RED);
            }

            for (int i = 0; i < ball_count; i++) {
                ball b = balls[i];
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    b.pos->x = b.pos->x + ((m_pos.x - b.pos->x) / 10) - random() % 7 + 3;
                    b.pos->y = b.pos->y + ((m_pos.y - b.pos->y) / 10) - random() % 7 + 3;
                } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
                    b.pos->x = b.pos->x - ((m_pos.x - b.pos->x) / 10) - random() % 7 + 3;
                    b.pos->y = b.pos->y - ((m_pos.y - b.pos->y) / 10) - random() % 7 + 3;
                } else {
                    container->calculate_hitbox(&container->data, &b);
                }
                DrawRing((Vector2){ b.pos->x, b.pos->y }, b.size - b.padding, b.size, 0, 360, -1, b.color);
            }
            if (IsKeyPressed(KEY_SPACE)) {
                free_balls(balls, ball_count);
                balls = create_balls(NULL, ++ball_count);
            }
        EndDrawing();
    }

    free_container(container);
    free_balls(balls, ball_count);

    CloseWindow();
    return 0;
}

ball* create_balls(ball* balls, int ball_count){
    if (balls == NULL) {
        balls = malloc(sizeof(ball) * ball_count);
    }

    for (int i = 0; i < ball_count; i++) {
        Vector2* spd = malloc(sizeof(Vector2));
        spd->x = GetRandomValue(-300, 300);
        spd->y = GetRandomValue(-300, 300);
        Vector2* pos = malloc(sizeof(Vector2));
        pos->x = GetRandomValue(CENTER.x - CONTAINER_SIZE / 4.0, CENTER.x + CONTAINER_SIZE / 4.0);
        pos->y = GetRandomValue(CENTER.y - CONTAINER_SIZE / 4.0, CENTER.y + CONTAINER_SIZE / 4.0);
        ball bd = {
            .speed = spd,
            .pos = pos, 
            .color = ColorFromHSV(GetRandomValue(0.0, 360.0), 0.7, 0.9),
            .size = 16,
            .padding = 8,
        };
        balls[i] = bd;
    }
    return balls;
}


void free_balls(ball* balls, int ball_count){
    for (int i = 0; i < ball_count; i++) {
        free(balls[i].speed);
        free(balls[i].pos);
    }
    free(balls);
    return;
}

void draw_debug_container() {
    DrawCircle(25,  50, 5, (Color){ 0, 0, 255, 255 });
    DrawCircle(25, 100, 5, (Color){ 0, 0, 240, 255 });
    DrawCircle(25, 150, 5, (Color){ 0, 0, 200, 255 });
    DrawCircle(25, 200, 5, (Color){ 0, 0, 160, 255 });
    DrawCircle(25, 250, 5, (Color){ 0, 0, 120, 255 });
    DrawCircle(25, 300, 5, (Color){ 0, 0,  80, 255 });
    DrawCircle(25, 350, 5, (Color){ 0, 0,  40, 255 });

    DrawCircle( 50, 25, 5, (Color){ 0, 255, 0, 255 });
    DrawCircle(100, 25, 5, (Color){ 0, 240, 0, 255 });
    DrawCircle(150, 25, 5, (Color){ 0, 200, 0, 255 });
    DrawCircle(200, 25, 5, (Color){ 0, 160, 0, 255 });
    DrawCircle(250, 25, 5, (Color){ 0, 120, 0, 255 });
    DrawCircle(300, 25, 5, (Color){ 0,  80, 0, 255 });
    DrawCircle(350, 25, 5, (Color){ 0,  40, 0, 255 });
}

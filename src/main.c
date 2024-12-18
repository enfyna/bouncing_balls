#include "raylib.h"
#include "container.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 400
#define WIDTH HEIGHT
#define CENTER (Vector2){WIDTH / 2.0, HEIGHT / 2.0}
#define BG_COLOR BLACK

#define TITLE "Bouncing Balls"
#define FPS 144.0

#define CONTAINER_PADDING 4
#define CONTAINER_WIDTH CONTAINER_SIZE + CONTAINER_PADDING * 2
#define CONTAINER_COLOR RAYWHITE

#define GRAVITY 1000 // 100.0

void draw_container(void);

ball* create_balls(ball* balls, int ball_count, short ball_size, short ball_padding, int line_len, int c_size);
void free_balls(ball* balls, int ball_count);

int main(int argc, char **argv){
    int ball_count = 10;
    short ball_size = 12;
    short ball_padding = 8;

    short c_shape = 0;
    float c_damp = 0.8;
    int c_size = 200;
    short line_mode = 0;
    int line_len = -1;

    float gravity = 1000.0;

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "circle")) {
            c_shape = 0;
        } else if (!strcmp(argv[i], "square")) {
            c_shape = 1;
        } else if (!strcmp(argv[i], "-cs")) {
            c_size = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-cd")) {
            c_damp = atof(argv[++i]);
        } else if (!strcmp(argv[i], "-clm")) {
            line_mode = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-g")) {
            gravity = atof(argv[++i]);
        } else if (!strcmp(argv[i], "-bp")) {
            ball_padding = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-bs")) {
            ball_size = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-bc")) {
            ball_count = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-ll")) {
            line_len = atoi(argv[++i]);
            if (line_len > INT_MAX / 2) {
                line_len = INT_MAX / 2;
            }
        }
    }

    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    Container* container = get_container(c_shape);
    ContainerData data = {
        .center = CENTER,
        .size = c_size,
        .padding = CONTAINER_PADDING,
        .shape = c_shape,
        .gravity = gravity,
        .damp = c_damp,
        .bg_color = BG_COLOR,
        .color = CONTAINER_COLOR,
        .line_mode = line_mode,
    };
    container->data = data;

    ball* balls = create_balls(NULL, ball_count, ball_size, ball_padding, line_len, c_size);
    bool isPaused = false;
    while (!WindowShouldClose() && !IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
            if (IsKeyPressed(KEY_Q)) {
                isPaused = !isPaused;
            }
            if (isPaused) {
                EndDrawing();
                continue;
            }
            ClearBackground(BG_COLOR);
            Vector2 m_pos = GetMousePosition();

            container->data.delta = GetFrameTime();

            container->draw(&container->data);

            for (int i = 0; i < ball_count; i++) {
                ball b = balls[i];
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    b.pos->x = b.pos->x + ((m_pos.x - b.pos->x) / 10) - GetRandomValue(7, 10);
                    b.pos->y = b.pos->y + ((m_pos.y - b.pos->y) / 10) - GetRandomValue(7, 10);
                } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
                    b.pos->x = b.pos->x - ((m_pos.x - b.pos->x) / 10) - GetRandomValue(7, 10);
                    b.pos->y = b.pos->y - ((m_pos.y - b.pos->y) / 10) - GetRandomValue(7, 10);
                } else {
                    container->calculate_hitbox(&container->data, balls + i);
                }
                DrawRing((Vector2){ b.pos->x, b.pos->y }, b.size - b.padding, b.size, 0, 360, -1, b.color);
            }
            if (IsKeyPressed(KEY_SPACE)) {
                free_balls(balls, ball_count);
                balls = create_balls(NULL, ++ball_count, ball_size, ball_padding, line_len, container->data.size);
            } else if (ball_count > 0 && IsKeyPressed(KEY_TAB)) {
                free_balls(balls, ball_count);
                balls = create_balls(NULL, --ball_count, ball_size, ball_padding, line_len, container->data.size);
            }
        EndDrawing();
    }

    free_container(container);
    free_balls(balls, ball_count);

    CloseWindow();
    return 0;
}

ball* create_balls(ball* balls, int ball_count, short ball_size, short ball_padding, int line_len, int c_size){
    if (balls == NULL) {
        balls = malloc(sizeof(ball) * ball_count);
    }

    for (int i = 0; i < ball_count; i++) {
        Vector2* spd = malloc(sizeof(Vector2));
        spd->x = GetRandomValue(-300, 300);
        spd->y = GetRandomValue(-300, 300);
        Vector2* pos = malloc(sizeof(Vector2));
        pos->x = GetRandomValue(CENTER.x - c_size / 4.0, CENTER.x + c_size / 4.0);
        pos->y = GetRandomValue(CENTER.y - c_size / 4.0, CENTER.y + c_size / 4.0);
        if (line_len == -1) {
            line_len = GetRandomValue(10, 1000);
        }
        ball bd = {
            .id = i,
            .points = malloc(sizeof(Vector2) * line_len),
            .line_len = line_len,
            .point_count = 0,
            .speed = spd,
            .pos = pos, 
            .color = ColorFromHSV(GetRandomValue(0.0, 360.0), 1.0, 1.0),
            .size = ball_size,
            .padding = ball_padding,
        };
        balls[i] = bd;
    }
    return balls;
}


void free_balls(ball* balls, int ball_count){
    for (int i = 0; i < ball_count; i++) {
        free(balls[i].points);
        free(balls[i].speed);
        free(balls[i].pos);
    }
    free(balls);
    return;
}

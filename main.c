#include "raylib.h"
#include "container.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 400
#define WIDTH HEIGHT
#define CENTER (Vector2){WIDTH / 2.0, HEIGHT / 2.0}
#define BG_COLOR BLACK

#define TITLE "Bouncing Balls"
#define FPS 60

// #define DEBUG

#define CONTAINER_SIZE 300
#define CONTAINER_PADDING 5
#define CONTAINER_WIDTH CONTAINER_SIZE + CONTAINER_PADDING * 2
#define CONTAINER_COLOR RAYWHITE

#define BALL_COUNT 25
#define BALL_SIZE 15
#define BALL_PADDING 2

#define GRAVITY 0.2

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct {
    Vector2 pos;
    Vector2 speed;
    Color color;
} ball;

void draw_container(void);

int main(int argc, char **argv){
    bool CONTAINER_SHAPE = 1;

    for (int i = 0; i < argc; i++) {
        printf("%s res: %d\n", argv[i], !strcmp(argv[i], "circle"));
        if (!strcmp(argv[i], "circle")) {
            CONTAINER_SHAPE = 0;
        } else if (!strcmp(argv[i], "square")) {
            CONTAINER_SHAPE = 1;
        }
    }

    Container* container = get_container(CONTAINER_SHAPE);
    ContainerData data = {
        .center = CENTER,
        .size = CONTAINER_SIZE,
        .padding = CONTAINER_PADDING,

        .gravity = GRAVITY,
        .ball_size = BALL_SIZE,

        .bg_color = BG_COLOR,
        .color = CONTAINER_COLOR,
    };
    container->data = &data;

    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    ball balls[BALL_COUNT] = {};
    Color colors[4] = { RED, GREEN, YELLOW, BLUE };

    for (int i = 0; i < BALL_COUNT; i++) {
        ball b = balls[i];
        b.speed = (Vector2){ random() % 10, random() % 10 };
        b.pos   = (Vector2){ random() % WIDTH, random() % HEIGHT };
        b.speed = (Vector2){ 0, 0 };
        b.pos   = (Vector2){ 200, 200 };
        b.color = colors[GetRandomValue(0, 3)];
        balls[i] = b;
    }

    double delta = 60.0 / FPS;

    bool user_wants_exit = false;

    while (!WindowShouldClose() && !user_wants_exit) {
        BeginDrawing();

            Vector2 m_pos = GetMousePosition();

            #ifndef DEBUG
                ClearBackground(BG_COLOR);
                container->draw(container->data, WIDTH, HEIGHT);
            #else
                const char* m_str = TextFormat("x: %d y: %d", (int)m_pos.x, (int)m_pos.y);
                draw_debug_container();
                DrawText(m_str, m_pos.x + 10, m_pos.y + 10, 20, WHITE);
                DrawCircle(m_pos.x, m_pos.y, 12, IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? BLUE : IsMouseButtonDown(MOUSE_RIGHT_BUTTON) ? GREEN : RED);
            #endif

            for (int i = 0; i < BALL_COUNT; i++) {
                ball b = balls[i];
                Vector2 b_pos = b.pos;

                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    b_pos.x = b_pos.x + ((m_pos.x - b_pos.x) / 10) - random() % 7 + 3;
                    b_pos.y = b_pos.y + ((m_pos.y - b_pos.y) / 10) - random() % 7 + 3;
                } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
                    b_pos.x = b_pos.x - ((m_pos.x - b_pos.x) / 10) - random() % 7 + 3;
                    b_pos.y = b_pos.y - ((m_pos.y - b_pos.y) / 10) - random() % 7 + 3;
                } else {
                    Vector2 b_spd = b.speed;

                    container->calculate_hitbox(container->data, &b_pos, &b_spd);

                    b_spd.y += GRAVITY;

                    b_pos.y += b_spd.y * delta;
                    b_pos.x += b_spd.x * delta;

                    balls[i].speed = b_spd;
                }
                balls[i].pos = b_pos;
                DrawCircle(b_pos.x, b_pos.y, BALL_SIZE + BALL_PADDING, WHITE);
                DrawCircle(b_pos.x, b_pos.y, BALL_SIZE, b.color);
            }

            if (IsKeyDown(KEY_ENTER)) {
                user_wants_exit = true;
            }

        EndDrawing();
    }
    CloseWindow();
    return 0;
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


#include "../conflict_physics.h"
#include <raylib.h>

#define COUNT 24
#define SPEED 3

int score = 0;
float velocity = 0;

enum {
    CATEGORY_PLAYER = 1 << 0,
    CATEGORY_WALL = 1 << 1,
    CATEGORY_COIN = 1 << 2,
};

void coinOnTrigger(CflBody* self, CflBody* trigger) {
    score++;
    self->isEnabled = false;
}

int main(void) {
    CflBody bodies[COUNT];

    // player
//    bodies[0] = cflCircle(100, 100, 10);
    bodies[0] = cflRectangle(100, 100, 20, 20);
    bodies[0].categoryMask = CATEGORY_PLAYER;
    bodies[0].collisionMask = CATEGORY_WALL;
    bodies[0].detectionMask = CATEGORY_COIN;

    // floor
    bodies[1] = cflRectangle(0, 400, 800, 50);
    bodies[1].isStatic = true;
    bodies[1].categoryMask = CATEGORY_WALL;

    // obstacles
    bodies[2] = cflRectangle(200, 200, 100, 100);
    bodies[2].categoryMask = CATEGORY_WALL;
    bodies[2].collisionMask = CATEGORY_WALL;

    bodies[3] = cflRectangle(400, 300, 50, 50);
    bodies[3].categoryMask = CATEGORY_WALL;
    bodies[3].collisionMask = CATEGORY_WALL;

    bodies[4] = cflRectangle(500, 300, 50, 50);
    bodies[4].categoryMask = CATEGORY_WALL;
    bodies[4].collisionMask = CATEGORY_WALL;

    bodies[5] = cflCircle(600, 300, 25);
    bodies[5].categoryMask = CATEGORY_WALL;
    bodies[5].collisionMask = CATEGORY_WALL;

    bodies[6] = cflCircle(700, 300, 25);
    bodies[6].categoryMask = CATEGORY_WALL;
    bodies[6].collisionMask = CATEGORY_WALL;

    // coins
    for (int i = 7; i < COUNT; i++) {
        bodies[i] = cflCircle((float)i * 40, (float)GetRandomValue(200, 400), 10);
        bodies[i].categoryMask = CATEGORY_COIN;
        bodies[i].detectionMask = CATEGORY_PLAYER;
        bodies[i].onTrigger = coinOnTrigger;
    }

    InitWindow(800, 450, "conflict_physics test");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_LEFT)) {
            bodies[0].x -= SPEED;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            bodies[0].x += SPEED;
        }
        if (IsKeyDown(KEY_UP)) {
            bodies[0].y -= SPEED;
        }
        if (IsKeyDown(KEY_DOWN)) {
            bodies[0].y += SPEED;
        }
        if (IsKeyDown(KEY_SPACE) && bodies[0].isOnFloor) {
            velocity = -5;
        }
        velocity += 0.1f;
        bodies[0].y += velocity;
        if (velocity > 5) {
            velocity = 5;
        }

        cflSolve(bodies, COUNT);
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawFPS(12, 12);
        // Draw score
        DrawText(TextFormat("Score: %i", score), 700, 12, 20, LIME);

        Color playerColor = SKYBLUE;
        if (bodies[0].isOnFloor) {
            playerColor = GREEN;
        } else if (bodies[0].isOnCeiling) {
            playerColor = RED;
        } else if (bodies[0].isOnWall) {
            playerColor = BLUE;
        }

//        DrawCircle((int) bodies[0].x, (int) bodies[0].y, bodies[0].collider.value.circle.radius, SKYBLUE);
        DrawRectangle((int)bodies[0].x, (int)bodies[0].y, (int)bodies[0].collider.value.rectangle.width, (int)bodies[0].collider.value.rectangle.height, playerColor);
        for (int i = 1; i < COUNT; i++) {
            if (!bodies[i].isEnabled) continue;
            if (bodies[i].collider.type == CFL_COLLIDER_TYPE_CIRCLE) {
                Color color = bodies[i].categoryMask == CATEGORY_COIN ? YELLOW : PINK;
                DrawCircle((int) bodies[i].x, (int) bodies[i].y, bodies[i].collider.value.circle.radius,color);
            } else if (bodies[i].collider.type == CFL_COLLIDER_TYPE_RECTANGLE) {
                DrawRectangle((int) bodies[i].x, (int) bodies[i].y, (int) bodies[i].collider.value.rectangle.width,
                              (int) bodies[i].collider.value.rectangle.height, PINK);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
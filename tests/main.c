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

void coinOnDetection(CflBody *self, CflBody *detected) {
    if (detected->categoryMask != CATEGORY_PLAYER) return;
    score++;
    self->isEnabled = false;
}

int main(void) {
    CflBody bodies[COUNT];
    int i = 0;
    // player
    bodies[0] = cflCircle(100, 100, 15);
    bodies[i].categoryMask = CATEGORY_PLAYER;
    bodies[i].collisionMask = CATEGORY_WALL;
    bodies[i].detectionMask = CATEGORY_COIN;
    i++;

    // walls
    bodies[i] = cflRectangle(0, 400, 800, 50);
    bodies[i].isStatic = true;
    bodies[i].categoryMask = CATEGORY_WALL;
    i++;

    bodies[i] = cflRectangle(0, 0, 50, 450);
    bodies[i].isStatic = true;
    bodies[i].categoryMask = CATEGORY_WALL;
    i++;

    bodies[i] = cflRectangle(750, 0, 50, 450);
    bodies[i].isStatic = true;
    bodies[i].categoryMask = CATEGORY_WALL;
    i++;

    bodies[i] = cflRectangle(0, 0, 800, 50);
    bodies[i].isStatic = true;
    bodies[i].categoryMask = CATEGORY_WALL;
    i++;

    // obstacles
    bodies[i] = cflRectangle(200, 200, 100, 100);
    bodies[i].categoryMask = CATEGORY_WALL;
    bodies[i].collisionMask = CATEGORY_WALL;
    i++;

    bodies[i] = cflRectangle(400, 300, 50, 50);
    bodies[i].categoryMask = CATEGORY_WALL;
    bodies[i].collisionMask = CATEGORY_WALL;
    i++;

    bodies[i] = cflRectangle(500, 300, 50, 50);
    bodies[i].categoryMask = CATEGORY_WALL;
    bodies[i].collisionMask = CATEGORY_WALL;
    i++;

    bodies[i] = cflCircle(600, 300, 25);
    bodies[i].categoryMask = CATEGORY_WALL;
    bodies[i].collisionMask = CATEGORY_WALL;
    i++;

    bodies[i] = cflCircle(700, 300, 25);
    bodies[i].categoryMask = CATEGORY_WALL;
    bodies[i].collisionMask = CATEGORY_WALL;
    i++;

    // coins
    float offset = 100;
    for (; i < COUNT; i++) {
        bodies[i] = cflCircle(offset, (float) GetRandomValue(75, 375), 8);
        bodies[i].categoryMask = CATEGORY_COIN;
        bodies[i].detectionMask = CATEGORY_PLAYER;
        bodies[i].onDetection = coinOnDetection;
        offset += 50;
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
        if (IsKeyDown(KEY_UP) && (bodies[0].isOnWall || bodies[0].isOnCeiling)) {
            bodies[0].y -= SPEED;
        }
        if (IsKeyDown(KEY_SPACE) && (bodies[0].isOnFloor || bodies[0].isOnWall)) {
            velocity = -4;
        }
        velocity += 0.1f;
        if (!bodies[0].isOnCeiling) {
            bodies[0].y += velocity;
        } else {
            velocity = 0;
        }
        if (velocity > 4) {
            velocity = 4;
        }

        cflSolve(bodies, COUNT);
        BeginDrawing();
        ClearBackground(DARKGRAY);

        Color playerColor = SKYBLUE;
        if (bodies[0].isOnFloor) {
            playerColor = GREEN;
        } else if (bodies[0].isOnCeiling) {
            playerColor = RED;
        } else if (bodies[0].isOnWall) {
            playerColor = BLUE;
        }

        DrawCircle((int) bodies[0].x, (int) bodies[0].y, bodies[0].collider.value.circle.radius, playerColor);
        for (i = 1; i < COUNT; i++) {
            if (!bodies[i].isEnabled) continue;
            if (bodies[i].collider.type == CFL_COLLIDER_TYPE_CIRCLE) {
                Color color = bodies[i].categoryMask == CATEGORY_COIN ? YELLOW : PINK;
                DrawCircle(
                    (int) bodies[i].x,
                    (int) bodies[i].y,
                    bodies[i].collider.value.circle.radius,
                    color
                );
            } else if (bodies[i].collider.type == CFL_COLLIDER_TYPE_RECTANGLE) {
                DrawRectangle(
                    (int) bodies[i].x,
                    (int) bodies[i].y,
                    (int) bodies[i].collider.value.rectangle.width,
                    (int) bodies[i].collider.value.rectangle.height,
                    PINK
                );
            }
        }

        DrawFPS(12, 12);
        DrawText(TextFormat("Score: %i", score), 700, 12, 20, LIME);
        EndDrawing();
    }

    CloseWindow();
}
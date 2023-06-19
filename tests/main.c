#include "../conflict_physics.h"
#include <raylib.h>

#include <stdio.h>

#define COUNT 3

// TODO: visual test with raylib

int main(void) {
    CflBody bodies[COUNT];

    // player body
//    bodies[0] = cflCircle(100, 100, 10);
    bodies[0] = cflRectangle(100, 100, 20, 20);
    bodies[0].categoryMask = 1;
    bodies[0].collisionMask = 2;
    bodies[0].onTrigger = NULL;

    // wall body
    bodies[1] = cflRectangle(200, 200, 100, 100);
//    bodies[1].isStatic = true;
    bodies[1].categoryMask = 2;

    // floor body
    bodies[2] = cflRectangle(0, 400, 800, 50);
    bodies[2].isStatic = true;
    bodies[2].categoryMask = 2;


    InitWindow(800, 450, "conflict_physics test");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_LEFT)) {
            bodies[0].x -= 1;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            bodies[0].x += 1;
        }
        if (IsKeyDown(KEY_UP)) {
            bodies[0].y -= 1;
        }
        if (IsKeyDown(KEY_DOWN)) {
            bodies[0].y += 1;
        }

        cflSolve(bodies, COUNT);
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawFPS(12, 12);

//        DrawCircle((int) bodies[0].x, (int) bodies[0].y, bodies[0].collider.value.circle.radius, SKYBLUE);
        DrawRectangle((int)bodies[0].x, (int)bodies[0].y, (int)bodies[0].collider.value.rectangle.width,
                      (int)bodies[0].collider.value.rectangle.height, SKYBLUE);
        for (int i = 1; i < COUNT; i++) {
            if (bodies[i].collider.type == CFL_COLLIDER_TYPE_CIRCLE) {
                DrawCircle((int) bodies[i].x, (int) bodies[i].y, bodies[i].collider.value.circle.radius, PINK);
            } else if (bodies[i].collider.type == CFL_COLLIDER_TYPE_RECTANGLE) {
                DrawRectangle((int) bodies[i].x, (int) bodies[i].y, (int) bodies[i].collider.value.rectangle.width,
                              (int) bodies[i].collider.value.rectangle.height, PINK);
            }
        }
        EndDrawing();
    }

    CloseWindow();
}
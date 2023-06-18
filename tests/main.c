#include "../conflict_physics.h"
#include <stdio.h>

#define COUNT 2

int main(void) {

    CflBody bodies[COUNT];
    bodies[0] = cflCircle(100, 100, 20);
    bodies[0].categoryMask = 1;
    bodies[0].collisionMask = 1;

    bodies[1] = cflCircle(110, 100, 20);
    bodies[1].categoryMask = 1;
    bodies[1].collisionMask = 1;

//    bodies[2] = cflRectangle(100, 110, 35, 40);
//    bodies[2].categoryMask = 1;
//    bodies[2].collisionMask = 1;
//
//    bodies[3] = cflRectangle(90, 100, 45, 50);
//    bodies[3].categoryMask = 1;
//    bodies[3].collisionMask = 1;

    // print the positions of all the bodies
    printf("initial positions:\n");
    int i;
    for (i = 0; i < COUNT; i++) {
        printf("body %d: (%f, %f)\n", i, bodies[i].x, bodies[i].y);
    }

    cflSolve(bodies, COUNT);

    printf("\nfinal positions:\n");
    for (i = 0; i < COUNT; i++) {
        printf("body %d: (%f, %f)\n", i, bodies[i].x, bodies[i].y);
    }
}
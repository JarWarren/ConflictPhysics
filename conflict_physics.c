#include "conflict_physics.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct Cfl1DCollision {
    uint32_t a;
    uint32_t b;
} Cfl1DCollision;

CflBody cflCircle(float x, float y, float radius) {
    CflBody body;
    body.x = x;
    body.y = y;
    body.isStatic = false;
    body.collider.type = CFL_COLLIDER_TYPE_CIRCLE;
    body.collider.value.circle.radius = radius;
    body.onDetection = NULL;
    body.categoryMask = 0;
    body.collisionMask = 0;
    body.detectionMask = 0;
    body.isEnabled = true;
    body.isOnCeiling = false;
    body.isOnFloor = false;
    body.isOnWall = false;
    return body;
}

CflBody cflRectangle(float x, float y, float width, float height) {
    CflBody body;
    body.x = x;
    body.y = y;
    body.isStatic = false;
    body.collider.type = CFL_COLLIDER_TYPE_RECTANGLE;
    body.collider.value.rectangle.width = width;
    body.collider.value.rectangle.height = height;
    body.onDetection = NULL;
    body.categoryMask = 0;
    body.collisionMask = 0;
    body.detectionMask = 0;
    body.isEnabled = true;
    body.isOnCeiling = false;
    body.isOnFloor = false;
    body.isOnWall = false;
    return body;
}

static void cflResolve(CflBody *a, CflBody *b, float x, float y) {
    if (!a->isStatic && !b->isStatic) {
        a->x += x / 2;
        a->y += y / 2;
        b->x -= x / 2;
        b->y -= y / 2;
    } else if (!a->isStatic) {
        a->x += x;
        a->y += y;
    } else if (!b->isStatic) {
        b->x -= x;
        b->y -= y;
    }

    if (y < 0) {
        a->isOnFloor = true;
        b->isOnCeiling = true;
    } else if (y > 0) {
        a->isOnCeiling = true;
        b->isOnFloor = true;
    }

    if (x != 0) {
        a->isOnWall = true;
        b->isOnWall = true;
    }
}

void cflSolve(CflBody *bodies, uint32_t count) {
    if (count < 2) return;

    // broad phase - sweep and prune
    uint32_t collision1DCapacity = count * 2;
    Cfl1DCollision* collisions1D = malloc(sizeof(Cfl1DCollision) * collision1DCapacity);
    uint32_t collisions1DCount = 0;

    uint32_t i;
    uint32_t j;
    for (i = 0; i < count; i++) {
        CflBody *a = &bodies[i];
        if (!a->isEnabled) continue;
        a->isOnCeiling = false;
        a->isOnFloor = false;
        a->isOnWall = false;
        float aMinX;
        float aMaxX;
        switch (a->collider.type) {
            case CFL_COLLIDER_TYPE_CIRCLE:
                aMinX = a->x - a->collider.value.circle.radius;
                aMaxX = a->x + a->collider.value.circle.radius;
                break;
            case CFL_COLLIDER_TYPE_RECTANGLE:
                aMinX = a->x;
                aMaxX = a->x + a->collider.value.rectangle.width;
                break;
        }
        for (j = i + 1; j < count; j++) {
            CflBody *b = &bodies[j];
            if (!b->isEnabled) continue;
            if (a->isStatic && b->isStatic) continue;
            float bMinX;
            float bMaxX;
            switch (b->collider.type) {
                case CFL_COLLIDER_TYPE_CIRCLE:
                    bMinX = b->x - b->collider.value.circle.radius;
                    bMaxX = b->x + b->collider.value.circle.radius;
                    break;
                case CFL_COLLIDER_TYPE_RECTANGLE:
                    bMinX = b->x;
                    bMaxX = b->x + b->collider.value.rectangle.width;
                    break;
            }

            if ((a->collisionMask & b->categoryMask) == 0 &&
                (a->detectionMask & b->categoryMask) == 0 &&
                (b->collisionMask & a->categoryMask) == 0 &&
                (b->detectionMask & a->categoryMask) == 0)
                continue;

            if (aMinX < bMaxX && aMaxX > bMinX) {
                if (collisions1DCount == collision1DCapacity) {
                    collision1DCapacity *= 2;
                    collisions1D = (Cfl1DCollision*)realloc(collisions1D, collision1DCapacity * sizeof(Cfl1DCollision));
                }
                collisions1D[collisions1DCount].a = i;
                collisions1D[collisions1DCount].b = j;
                collisions1DCount++;
            }
        }
    }

    // narrow phase
    for (i = 0; i < collisions1DCount; i++) {
        Cfl1DCollision *collision = &collisions1D[i];
        CflBody *a = &bodies[collision->a];
        CflBody *b = &bodies[collision->b];
        if (a->collider.type == CFL_COLLIDER_TYPE_CIRCLE && b->collider.type == CFL_COLLIDER_TYPE_CIRCLE) {
            float dx = a->x - b->x;
            float dy = a->y - b->y;
            float distance = sqrtf(dx * dx + dy * dy);
            float overlap = (a->collider.value.circle.radius + b->collider.value.circle.radius) - distance;
            if (overlap <= 0) continue;
            bool isDetection = false;
            if ((a->detectionMask & b->categoryMask) != 0 && a->onDetection != NULL) {
                a->onDetection(a, b);
                isDetection = true;
            }
            if ((b->detectionMask & a->categoryMask) != 0 && b->onDetection != NULL) {
                b->onDetection(b, a);
                isDetection = true;
            }
            if (isDetection) continue;
            cflResolve(a, b, dx / distance * overlap, dy / distance * overlap);
        } else if (a->collider.type == CFL_COLLIDER_TYPE_RECTANGLE && b->collider.type == CFL_COLLIDER_TYPE_RECTANGLE) {
            float overlapX;
            float overlapY;
            if (a->x < b->x) {
                overlapX = a->x + a->collider.value.rectangle.width - b->x;
            } else {
                overlapX = b->x + b->collider.value.rectangle.width - a->x;
            }
            if (a->y < b->y) {
                overlapY = a->y + a->collider.value.rectangle.height - b->y;
            } else {
                overlapY = b->y + b->collider.value.rectangle.height - a->y;
            }
            if (overlapX <= 0 || overlapY <= 0) continue;
            bool isDetection = false;
            if ((a->detectionMask & b->categoryMask) != 0 && a->onDetection != NULL) {
                a->onDetection(a, b);
                isDetection = true;
            }
            if ((b->detectionMask & a->categoryMask) != 0 && b->onDetection != NULL) {
                b->onDetection(b, a);
                isDetection = true;
            }
            if (isDetection) continue;
            if (overlapX < overlapY) {
                if (a->x < b->x) {
                    cflResolve(a, b, -overlapX, 0);
                } else {
                    cflResolve(a, b, overlapX, 0);
                }
            } else {
                if (a->y < b->y) {
                    cflResolve(a, b, 0, -overlapY);
                } else {
                    cflResolve(a, b, 0, overlapY);
                }
            }
        } else {
            CflBody *circle;
            CflBody *rectangle;
            bool isReversed = false;
            if (a->collider.type == CFL_COLLIDER_TYPE_CIRCLE) {
                circle = a;
                rectangle = b;
            } else {
                isReversed = true;
                circle = b;
                rectangle = a;
            }

            float nearestX;
            float nearestY;
            if (circle->x < rectangle->x) {
                nearestX = rectangle->x;
            } else if (circle->x > rectangle->x + rectangle->collider.value.rectangle.width) {
                nearestX = rectangle->x + rectangle->collider.value.rectangle.width;
            } else {
                nearestX = circle->x;
            }

            if (circle->y < rectangle->y) {
                nearestY = rectangle->y;
            } else if (circle->y > rectangle->y + rectangle->collider.value.rectangle.height) {
                nearestY = rectangle->y + rectangle->collider.value.rectangle.height;
            } else {
                nearestY = circle->y;
            }

            float dx = circle->x - nearestX;
            float dy = circle->y - nearestY;
            float distance = sqrtf(dx * dx + dy * dy);
            float overlap = circle->collider.value.circle.radius - distance;
            if (overlap <= 0 || distance == 0) continue;

            bool isDetection = false;
            if ((a->detectionMask & b->categoryMask) != 0 && a->onDetection != NULL) {
                a->onDetection(a, b);
                isDetection = true;
            }
            if ((b->detectionMask & a->categoryMask) != 0 && b->onDetection != NULL) {
                b->onDetection(b, a);
                isDetection = true;
            }
            if (isDetection) continue;

            cflResolve(
                isReversed ? b : a,
                isReversed ? a : b,
                dx / distance * overlap,
                dy / distance * overlap
            );
        }
    }
    free(collisions1D);
}

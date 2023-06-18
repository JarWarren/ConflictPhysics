#ifndef IMPHYSICS_H
#define IMPHYSICS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum ImBodyType ImBodyType;
typedef struct ImBody ImBody;
typedef struct ImCollider ImCollider;
typedef void (*ImPhysicsTriggerCallback)(ImBody* trigger);

enum ImBodyType {
    IM_BODY_TYPE_STATIC,
    IM_BODY_TYPE_DYNAMIC,
};

struct ImCollider {
    enum {
        IM_COLLIDER_TYPE_CIRCLE,
        IM_COLLIDER_TYPE_RECTANGLE,
    } type;
    union {
        struct {
            float radius;
        } circle;
        struct {
            float width;
            float height;
        } rectangle;
    } value;
};

struct ImBody {
    ImBodyType type;
    ImCollider collider;
    ImPhysicsTriggerCallback onTrigger;
    bool isEnabled;
    bool isOnCeiling;
    bool isOnFloor;
    bool isOnLeftWall;
    bool isOnRightWall;
    float x;
    float y;
    uint32_t categoryMask;
    uint32_t collisionMask;
    uint32_t triggerMask;
};

// TODO: body creation helper functions

void imphysics_solve(ImBody* bodies, uint32_t count);

#endif //IMPHYSICS_H

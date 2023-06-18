#ifndef IMPHYSICS_H
#define IMPHYSICS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum CflBodyType CflBodyType;
typedef struct CflBody CflBody;
typedef struct CflCollider CflCollider;
typedef void (*CflPhysicsTriggerCallback)(CflBody* trigger);

struct CflCollider {
    enum {
        CFL_COLLIDER_TYPE_CIRCLE,
        CFL_COLLIDER_TYPE_RECTANGLE,
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

struct CflBody {
    float x;
    float y;
    bool isStatic;
    CflCollider collider;
    CflPhysicsTriggerCallback onTrigger;
    uint32_t categoryMask;
    uint32_t collisionMask;
    uint32_t detectionMask;
    bool isEnabled;
    bool isOnCeiling;
    bool isOnFloor;
    bool isOnWall;
};

// TODO: body creation helper functions

void conflict_solve(CflBody* bodies, uint32_t count);

#endif //IMPHYSICS_H

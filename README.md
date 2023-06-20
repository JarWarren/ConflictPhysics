# Conflict Physics

Collision and overlap detection.

I write a lot of small games and most of them don't need a full physics engine.
I don't want to mess with acceleration, friction, mass, forces, etc.
I just want to tell things where to go and make be sure they won't overlap if they're not supposed to.

Conflict lets me do exactly that. Put your bodies where you want them and let it resolve the "conflicts".

## Features
- C99 and no dependencies
- Collisions
- Detections
- isOnWall, isOnFloor or isOnCeiling
- You own your body instances

## Usage
Because you own the bodies, you can put them wherever you want (works well with ecs). A minimal example
```c
#include "conflict_physics.h"
int main(void) {
    CflBody bodies[COUNT];
    bodies[0] = cflCircle(x, y, radius); // your player
    bodies[1] = cflRectangle(x, y, width, height); // the ground
    // etc
    ...
    
    while (yourGameLoop) {
        yourUpdate();
        cflSolve(bodies, COUNT);
        yourDraw();
    }
    
    yourTerminate();
}
```

## Example
 - `mkdir build && cd build`
 - `cmake ..`
 - `make`
 - `./example/ConflictPhysicsExample`
# Conflict Physics

Collision and overlap detection.

Many games don't need a full physics engine - they need to know when objects collide or overlap.
Don't mess with acceleration, friction, mass, forces or even delta time.

Just move your bodies directly and Conflict will resolve "conflicts".

## Features
- 2D or 3D
- C99 and no dependencies
- Dead easy to use

## Internally Managed Bodies
Let Conflict manage your bodies for you.
```c
#include "conflict_physics.h"
```

## Manually Managed Bodies
You own the bodies and Conflict just tells you when they conflict.
Good for when you want to use ecs or a custom allocator.
```c
#include "conflict_physics.h"
```
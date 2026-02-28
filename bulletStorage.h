#pragma once
#include "position.h"

/*********************************************
 * BULLET TYPE
 *********************************************/
enum class BulletType
{
    PELLET,
    BOMB,
    SHRAPNEL,
    MISSILE
};

/*********************************************
 * BULLET STORAGE
 * State only. No OpenGL. No rules.
 *********************************************/
class BulletStorage
{
public:
    BulletStorage()
        : pt(),
        v(),
        radius(5.0),
        dead(false),
        value(1),
        type(BulletType::PELLET),
        timeToDie(0)
    {
    }

    // ---- game state ----
    Position pt;        // position
    Velocity v;         // velocity
    double radius;      // collision radius
    bool dead;          // dead flag
    int value;          // score value/cost
    BulletType type;    // which bullet is this?
    int timeToDie;      // bomb/shrapnel lifetime counter
};
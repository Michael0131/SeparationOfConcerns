#pragma once
#include "position.h" // position and velocity

/*********************************************
 * EFFECT TYPE
 *********************************************/
enum class EffectType
{
    FRAGMENT,
    STREEK,
    EXHAUST
};

/*********************************************
 * EFFECT STORAGE
 * State only. No OpenGL. No rules.
 *********************************************/
class EffectStorage
{
public:
    EffectStorage()
        : pt(),
        age(0.5),
        type(EffectType::FRAGMENT),
        v(),
        size(0.0),
        ptEnd()
    {
    }

    // ---- shared ----
    Position pt;      // location of the effect
    double age;       // 1.0 = new, 0.0 = dead
    EffectType type;

    // ---- variant-specific ----
    Velocity v;       // Fragment only
    double size;      // Fragment only
    Position ptEnd;   // Streek/Exhaust only
};
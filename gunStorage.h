#pragma once
#include "position.h"

/*********************************************
 * GUN STORAGE
 * Stores all game state for the gun.
 * No OpenGL. No rules. Just data.
 *********************************************/
class GunStorage
{
public:
    GunStorage(const Position& pt)
        : angle(0.78 /* ~45 degrees */), pt(pt)
    {
    }

    // read-only access (safe for interface + logic)
    double getAngle() const { return angle; }
    const Position& getPosition() const { return pt; }

    // write access (intended for logic)
    void setAngle(double newAngle) { angle = newAngle; }

private:
    double angle;   // game state
    Position pt;    // game state
};
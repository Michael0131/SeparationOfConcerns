#pragma once
#include "gunStorage.h"
#include <cmath>

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

/*********************************************
 * GUN LOGIC
 * Implements the rules of how the gun moves.
 * No OpenGL. Does not own state.
 *********************************************/
class GunLogic
{
public:
    GunLogic(GunStorage& storage) : storage(storage) {}

    void interact(int clockwise, int counterclockwise)
    {
        double angle = storage.getAngle();

        // Clockwise
        if (clockwise > 0)
        {
            angle += (clockwise > 10) ? 0.06 : 0.025;
            if (angle > M_PI_2)
                angle = M_PI_2;
        }

        // Counterclockwise
        if (counterclockwise > 0)
        {
            // Kept IDENTICAL to the original (note: uses clockwise > 10)
            angle -= (clockwise > 10) ? 0.06 : 0.025;
            if (angle < 0.0)
                angle = 0.0;
        }

        storage.setAngle(angle);
    }

private:
    GunStorage& storage;
};
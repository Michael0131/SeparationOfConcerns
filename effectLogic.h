#pragma once
#include "effectStorage.h"
#include <cassert>
#include <cstdlib> // rand()

/*********************************************
 * EFFECT LOGIC
 * Rules only. No OpenGL. Does not own state.
 *********************************************/
class EffectLogic
{
public:
    explicit EffectLogic(EffectStorage& storage) : s(storage) {}

    // Generates a random number
    static double randomDoubleInclusiveMax(double min, double max)
    {
        assert(min <= max);
        double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
        assert(min <= num && num <= max);
        return num;
    }

    // Constructors
    static void initBase(EffectStorage& s, const Position& pt)
    {
        s.pt = pt;
        s.age = 0.5;
    }

    static void initFragment(EffectStorage& s, const Position& pt, const Velocity& vIn)
    {
        initBase(s, pt);
        s.type = EffectType::FRAGMENT;

        // velocity = random kick + 0.5 * incoming
        s.v.setDx(vIn.getDx() * 0.5 + randomDoubleInclusiveMax(-6.0, 6.0));
        s.v.setDy(vIn.getDy() * 0.5 + randomDoubleInclusiveMax(-6.0, 6.0));

        // age and size
        s.age = randomDoubleInclusiveMax(0.4, 1.0);
        s.size = randomDoubleInclusiveMax(1.0, 2.5);
    }

    static void initStreek(EffectStorage& s, const Position& pt, Velocity v)
    {
        initBase(s, pt);
        s.type = EffectType::STREEK;

        s.ptEnd = pt;
        v *= -1.0;
        s.ptEnd += v;

        s.age = 0.5;
    }

    static void initExhaust(EffectStorage& s, const Position& pt, Velocity v)
    {
        initBase(s, pt);
        s.type = EffectType::EXHAUST;

        s.ptEnd = pt;
        v *= -1.0;
        s.ptEnd += v;

        s.age = 0.5;
    }

    // Methods
    void fly()
    {
        switch (s.type)
        {
        case EffectType::FRAGMENT:
            s.pt += s.v;      // inertia
            s.age -= 0.02;    // fade
            s.size *= 0.95;   // shrink
            break;

        case EffectType::STREEK:
            s.age -= 0.10;
            break;

        case EffectType::EXHAUST:
            s.age -= 0.025;
            break;
        }
    }

    bool isDead() const
    {
        return s.age <= 0.0;
    }

private:
    EffectStorage& s;
};
#pragma once
#include "bulletStorage.h"
#include "effectInterface.h"
#include <list>
#include <cassert>
#include <cstdlib>  // rand()
#include <cmath>    // sin/cos

/*********************************************
 * BULLET LOGIC
 * Rules only. No OpenGL. Does not own state.
 *********************************************/
class BulletLogic
{
public:
    BulletLogic(BulletStorage& storage) : s(storage) {}

    // Matches original Bullet constructor behavior EXACTLY
    static void initializeLikeCtor(BulletStorage& s,
        const Position& dimensions,
        BulletType type,
        double angle,
        double speed,
        double radius,
        int value)
    {
        s.dead = false;
        s.type = type;
        s.radius = radius;
        s.value = value;

        // initial position
        s.pt.setX(dimensions.getX() - 1.0);
        s.pt.setY(1.0);
        assert(s.pt.getX() > 100.0);

        // initial velocity
        s.v.setDx(-speed * cos(angle));
        s.v.setDy(speed * sin(angle));
        assert(s.v.getDx() <= 0.0);
        assert(s.v.getDy() >= 0.0);
    }

    // Same random behavior as original Bullet::random(int,int)
    // NOTE: max is effectively EXCLUSIVE
    static int randomIntExclusiveMax(int min, int max)
    {
        assert(min < max);
        int num = (rand() % (max - min)) + min;
        assert(min <= num && num <= max);
        return num;
    }

    // Same random behavior as original Bullet::random(double,double)
    // NOTE: this can return max when rand()==RAND_MAX (same as original)
    static double randomDoubleInclusiveMax(double min, double max)
    {
        assert(min <= max);
        double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
        assert(min <= num && num <= max);
        return num;
    }

    // Missile steering rules (same as original Missile::input)
    void input(bool isUp, bool isDown, bool /*isB*/)
    {
        if (s.type != BulletType::MISSILE)
            return;

        if (isUp)
            s.v.turn(0.04);
        if (isDown)
            s.v.turn(-0.04);
    }

    // Move rules for all bullet types (same ordering as original)
    void move(const Position& dimensions, std::list<Effect*>& effects)
    {
        // Type-specific pre-move rules
        if (s.type == BulletType::BOMB)
        {
            s.timeToDie--;
            if (!s.timeToDie)
                s.dead = true;
        }
        else if (s.type == BulletType::MISSILE)
        {
            effects.push_back(new Exhaust(s.pt, s.v));
        }
        else if (s.type == BulletType::SHRAPNEL)
        {
            s.timeToDie--;
            if (!s.timeToDie)
                s.dead = true;

            effects.push_back(new Streek(s.pt, s.v));
        }

        // Base Bullet::move behavior (inertia then out-of-bounds kill)
        s.pt.add(s.v);

        if (isOutOfBounds(dimensions))
            s.dead = true;
    }

    // Logic decides what spawns on death (rule lives here).
    // Interface will construct objects from these storage snapshots.
    std::list<BulletStorage> deathSpawns() const
    {
        std::list<BulletStorage> spawned;

        // Only bombs spawn shrapnel (same as original Bomb::death)
        if (s.type != BulletType::BOMB)
            return spawned;

        for (int i = 0; i < 20; i++)
        {
            BulletStorage sh;
            sh.type = BulletType::SHRAPNEL;
            sh.dead = false;

            // Shrapnel ctor logic preserved:
            sh.timeToDie = randomIntExclusiveMax(5, 15); // same as original random(5,15)

            // The speed and direction is random (matches original v.set(angle, speed))
            sh.v.set(randomDoubleInclusiveMax(0.0, 6.2),
                randomDoubleInclusiveMax(10.0, 15.0));

            sh.pt = s.pt;
            sh.value = 0;
            sh.radius = 3.0;

            spawned.push_back(sh);
        }

        return spawned;
    }

private:
    BulletStorage& s;

    bool isOutOfBounds(const Position& dimensions) const
    {
        return (s.pt.getX() < -s.radius || s.pt.getX() >= dimensions.getX() + s.radius ||
            s.pt.getY() < -s.radius || s.pt.getY() >= dimensions.getY() + s.radius);
    }
};
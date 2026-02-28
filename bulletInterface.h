#pragma once
#include "bulletLogic.h"
#include <list>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL/glut.h>
#endif

// Forward declare factory (defined at bottom after all classes exist)
class Bullet;
Bullet* makeBulletFromStorage(const BulletStorage& s);

/*********************************************
 * BULLET INTERFACE (BASE)
 * Owns storage + composes logic.
 * Drawing only. Forwards rules to logic.
 *********************************************/
class Bullet
{
public:
    static Position dimensions;   // defined in game.cpp like before

public:
    virtual ~Bullet() = default;

    // Setters
    void kill() { storage.dead = true; }
    void setValue(int newValue) { storage.value = newValue; }

    // Getters
    bool isDead() const { return storage.dead; }
    Position getPosition() const { return storage.pt; }
    Velocity getVelocity() const { return storage.v; }
    double getRadius() const { return storage.radius; }
    int getValue() const { return storage.value; }

    // Input: only missile cares (logic decides)
    virtual void input(bool isUp, bool isDown, bool isB)
    {
        (void)isB;
        logic.input(isUp, isDown, isB);
    }

    // Movement: logic does all rules; interface just forwards
    virtual void move(std::list<Effect*>& effects)
    {
        logic.move(dimensions, effects);
    }

    // Death: default is no spawn (matches original Bullet::death{})
    virtual void death(std::list<Bullet*>& /*bullets*/) {}

    // Drawing: pure virtual
    virtual void output() = 0;

protected:
    Bullet() : storage(), logic(storage) {}

    // Used for constructing bullets from spawn-storage
    Bullet(const BulletStorage& existing) : storage(existing), logic(storage) {}

protected:
    BulletStorage storage;
    BulletLogic logic;

protected:
    // ---- Drawing helpers (interface-only) ----
    static inline void glVertexPoint(const Position& point)
    {
        glVertex2f((GLfloat)point.getX(), (GLfloat)point.getY());
    }

    void drawLine(const Position& begin, const Position& end,
        double red = 1.0, double green = 1.0, double blue = 1.0) const
    {
        glBegin(GL_LINES);
        glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);
        glVertexPoint(begin);
        glVertexPoint(end);
        glColor3f((GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0);
        glEnd();
    }

    void drawDot(const Position& point, double radius = 2.0,
        double red = 1.0, double green = 1.0, double blue = 1.0) const
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);
        double r = radius / 2.0;

        glVertex2f((GLfloat)(point.getX() - r), (GLfloat)(point.getY() - r));
        glVertex2f((GLfloat)(point.getX() + r), (GLfloat)(point.getY() - r));
        glVertex2f((GLfloat)(point.getX() + r), (GLfloat)(point.getY() + r));
        glVertex2f((GLfloat)(point.getX() - r), (GLfloat)(point.getY() + r));

        glColor3f((GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0);
        glEnd();
    }
};

/*********************
 * PELLET
 **********************/
class Pellet : public Bullet
{
public:
    Pellet(double angle, double speed = 15.0)
    {
        BulletLogic::initializeLikeCtor(storage, dimensions, BulletType::PELLET,
            angle, speed, 1.0, 1);
    }

    Pellet(const BulletStorage& s) : Bullet(s) {}

    void output() override
    {
        if (!isDead())
            drawDot(storage.pt, 3.0, 1.0, 1.0, 0.0);
    }
};

/*********************
 * BOMB
 **********************/
class Bomb : public Bullet
{
public:
    Bomb(double angle, double speed = 10.0)
    {
        BulletLogic::initializeLikeCtor(storage, dimensions, BulletType::BOMB,
            angle, speed, 4.0, 4);
        storage.timeToDie = 60; // same as original
    }

    Bomb(const BulletStorage& s) : Bullet(s) {}

    void output() override
    {
        if (!isDead())
        {
            drawDot(storage.pt, storage.radius + 2.0, 0.50, 0.50, 0.00);
            drawDot(storage.pt, storage.radius + 1.0, 0.75, 0.75, 0.00);
            drawDot(storage.pt, storage.radius + 0.0, 0.87, 0.87, 0.00);
            drawDot(storage.pt, storage.radius - 1.0, 1.00, 1.00, 0.00);
        }
    }

    // Interface performs construction; Logic decides what spawns.
    void death(std::list<Bullet*>& bullets) override
    {
        auto spawned = logic.deathSpawns();
        for (const auto& st : spawned)
            bullets.push_back(makeBulletFromStorage(st));
    }
};

/*********************
 * SHRAPNEL
 **********************/
class Shrapnel : public Bullet
{
public:
    Shrapnel(const BulletStorage& s) : Bullet(s) {}

    void output() override
    {
        if (!isDead())
            drawDot(storage.pt, storage.radius, 1.0, 1.0, 0.0);
    }
};

/*********************
 * MISSILE
 **********************/
class Missile : public Bullet
{
public:
    Missile(double angle, double speed = 10.0)
    {
        BulletLogic::initializeLikeCtor(storage, dimensions, BulletType::MISSILE,
            angle, speed, 1.0, 3);
    }

    Missile(const BulletStorage& s) : Bullet(s) {}

    void output() override
    {
        if (!isDead())
        {
            Position ptNext(storage.pt);
            ptNext.add(storage.v);
            drawLine(storage.pt, ptNext, 1.0, 1.0, 0.0);
            drawDot(storage.pt, 3.0, 1.0, 1.0, 1.0);
        }
    }
};

/*********************************************
 * FACTORY (Interface-level integration)
 * Defined AFTER all derived classes exist.
 *********************************************/
inline Bullet* makeBulletFromStorage(const BulletStorage& s)
{
    switch (s.type)
    {
    case BulletType::SHRAPNEL: return new Shrapnel(s);
    case BulletType::BOMB:     return new Bomb(s);
    case BulletType::MISSILE:  return new Missile(s);
    case BulletType::PELLET:
    default:                   return new Pellet(s);
    }
}
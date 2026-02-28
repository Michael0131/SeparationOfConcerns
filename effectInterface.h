#pragma once
#include "effectLogic.h"

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

/**********************
 * Effect: stuff that is not interactive
 * Owns storage + composes logic. Drawing only.
 **********************/
class Effect
{
public:
    virtual ~Effect() = default;

    // draw it
    virtual void render() const = 0;

    // move it forward with regards to inertia. Let it age
    virtual void fly() = 0;

    // it is dead when age goes to 0.0
    bool isDead() const { return logic.isDead(); }

protected:
    explicit Effect(const Position& pt) : storage(), logic(storage)
    {
        EffectLogic::initBase(storage, pt); // sets pt + age=0.5
    }

protected:
    EffectStorage storage;
    EffectLogic logic;
};

/**********************
 * FRAGMENT
 **********************/
class Fragment : public Effect
{
public:
    Fragment(const Position& pt, const Velocity& v) : Effect(pt)
    {
        EffectLogic::initFragment(storage, pt, v);
    }

    void render() const override
    {
        if (isDead())
            return;

        glBegin(GL_TRIANGLE_FAN);

        // color is function of age (fading to black)
        glColor3f((GLfloat)storage.age, (GLfloat)storage.age, (GLfloat)storage.age);

        // draw the fragment (square)
        glVertex2f((GLfloat)(storage.pt.getX() - storage.size), (GLfloat)(storage.pt.getY() - storage.size));
        glVertex2f((GLfloat)(storage.pt.getX() + storage.size), (GLfloat)(storage.pt.getY() - storage.size));
        glVertex2f((GLfloat)(storage.pt.getX() + storage.size), (GLfloat)(storage.pt.getY() + storage.size));
        glVertex2f((GLfloat)(storage.pt.getX() - storage.size), (GLfloat)(storage.pt.getY() + storage.size));

        glColor3f((GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0);
        glEnd();
    }

    void fly() override
    {
        logic.fly();
    }
};

/**********************
 * STREEK
 **********************/
class Streek : public Effect
{
public:
    Streek(const Position& pt, Velocity v) : Effect(pt)
    {
        EffectLogic::initStreek(storage, pt, v);
    }

    void render() const override
    {
        if (isDead())
            return;

        glBegin(GL_LINES);
        glColor3f((GLfloat)storage.age, (GLfloat)storage.age, (GLfloat)storage.age);

        glVertex2f((GLfloat)storage.pt.getX(), (GLfloat)storage.pt.getY());
        glVertex2f((GLfloat)storage.ptEnd.getX(), (GLfloat)storage.ptEnd.getY());

        glColor3f((GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0);
        glEnd();
    }

    void fly() override
    {
        logic.fly();
    }
};

/**********************
 * EXHAUST
 **********************/
class Exhaust : public Effect
{
public:
    Exhaust(const Position& pt, Velocity v) : Effect(pt)
    {
        EffectLogic::initExhaust(storage, pt, v);
    }

    void render() const override
    {
        if (isDead())
            return;

        glBegin(GL_LINES);
        glColor3f((GLfloat)storage.age, (GLfloat)storage.age, (GLfloat)storage.age);

        glVertex2f((GLfloat)storage.pt.getX(), (GLfloat)storage.pt.getY());
        glVertex2f((GLfloat)storage.ptEnd.getX(), (GLfloat)storage.ptEnd.getY());

        glColor3f((GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0);
        glEnd();
    }

    void fly() override
    {
        logic.fly();
    }
};
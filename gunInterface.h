#pragma once
#include "gunLogic.h"

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

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

/*********************************************
 * GUN INTERFACE
 * Handles all drawing and input plumbing.
 * Owns Storage and composes Logic.
 *********************************************/
class GunInterface
{
public:
    GunInterface(const Position& pt)
        : storage(pt), logic(storage)
    {
    }

    void interact(int clockwise, int counterclockwise)
    {
        logic.interact(clockwise, counterclockwise);
    }

    void display() const
    {
        drawRectangle(storage.getPosition(),
            M_PI_2 - storage.getAngle(),
            10.0, 100.0,
            1.0, 1.0, 1.0);
    }

    // Skeet uses this to aim bullets
    double getAngle() const { return storage.getAngle(); }

private:
    GunStorage storage;
    GunLogic logic;

private:
    // ---- drawing helpers: interface-only ----

    static Position rotate(const Position& origin, double x, double y, double rotation)
    {
        double cosA = cos(rotation);
        double sinA = sin(rotation);

        Position ptReturn(origin);
        ptReturn.addX(x * cosA - y * sinA);
        ptReturn.addY(y * cosA + x * sinA);
        return ptReturn;
    }

    static void glVertexPoint(const Position& point)
    {
        glVertex2f((GLfloat)point.getX(), (GLfloat)point.getY());
    }

    static void drawRectangle(const Position& pt,
        double angle = 0.0,
        double width = 10.0,
        double height = 100.0,
        double red = 1.0,
        double green = 1.0,
        double blue = 1.0)
    {
        glBegin(GL_QUADS);
        glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);

        glVertexPoint(rotate(pt, width / 2.0, height / 2.0, angle));
        glVertexPoint(rotate(pt, width / 2.0, -height / 2.0, angle));
        glVertexPoint(rotate(pt, -width / 2.0, -height / 2.0, angle));
        glVertexPoint(rotate(pt, -width / 2.0, height / 2.0, angle));
        glVertexPoint(rotate(pt, width / 2.0, height / 2.0, angle));

        glColor3f((GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0);
        glEnd();
    }
};
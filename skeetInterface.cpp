/**********************************************************************
 * Skeet Interface
 * Input + Output only
 **********************************************************************/

#include "SkeetInterface.h"
#include <string>
#include <sstream>
using namespace std;

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>
#include <GLUT/glut.h>
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_18
#endif

#ifdef __linux__
#include <GL/gl.h>
#include <GL/glut.h>
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif

/*************************************************************************
 * DRAW TEXT
 *************************************************************************/
static void drawText(const Position& topLeft, const char* text)
{
    void* pFont = GLUT_TEXT;
    glColor3f((GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0);

    glRasterPos2f((GLfloat)topLeft.getX(), (GLfloat)topLeft.getY());

    for (const char* p = text; *p; p++)
        glutBitmapCharacter(pFont, *p);
}

static void drawText(const Position& topLeft, const string& text)
{
    drawText(topLeft, text.c_str());
}

/*********************************************
 * SKEET INTERFACE : CTOR
 *********************************************/
SkeetInterface::SkeetInterface(Position& dimensions)
    : storage(dimensions), logic()
{
}

/************************
 * SKEET INTERFACE : ANIMATE
 ************************/
void SkeetInterface::animate()
{
    logic.animate(storage);
}

/************************
 * SKEET INTERFACE : INTERACT
 ************************/
void SkeetInterface::interact(const UserInput& ui)
{
    // reset the game
    if (storage.time.isGameOver() && ui.isSpace())
    {
        storage.time.reset();
        storage.score.reset();
        storage.hitRatio.reset();
        return;
    }

    // gather input from the interface
    storage.gun.interact(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft());
    Bullet* p = nullptr;

    // a pellet can be shot at any time
    if (ui.isSpace())
        p = new Pellet(storage.gun.getAngle());
    // missiles can be shot at level 2 and higher
    else if (ui.isM() && storage.time.level() > 1)
        p = new Missile(storage.gun.getAngle());
    // bombs can be shot at level 3 and higher
    else if (ui.isB() && storage.time.level() > 2)
        p = new Bomb(storage.gun.getAngle());

    storage.bullseye = ui.isShift();

    // add something if something has been added
    if (nullptr != p)
        storage.bullets.push_back(p);

    // send movement information to all the bullets. Only the missile cares.
    for (auto bullet : storage.bullets)
        bullet->input(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft(), ui.isB());
}

/************************************************************************
 * DRAW Background
 *************************************************************************/
void SkeetInterface::drawBackground(double redBack, double greenBack, double blueBack) const
{
    glBegin(GL_TRIANGLE_FAN);

    glColor3f((GLfloat)redBack, (GLfloat)greenBack, (GLfloat)blueBack);
    glVertex2f((GLfloat)0.0, (GLfloat)0.0);
    glVertex2f((GLfloat)storage.dimensions.getX(), (GLfloat)0.0);
    glVertex2f((GLfloat)storage.dimensions.getX(), (GLfloat)storage.dimensions.getY());
    glVertex2f((GLfloat)0.0, (GLfloat)storage.dimensions.getY());

    glEnd();
}

/************************************************************************
 * DRAW Timer
 *************************************************************************/
void SkeetInterface::drawTimer(double percent,
    double redFore, double greenFore, double blueFore,
    double redBack, double greenBack, double blueBack) const
{
    double radians;

    GLfloat length = (GLfloat)storage.dimensions.getX();
    GLfloat half = length / (GLfloat)2.0;

    drawBackground(redBack, greenBack, blueBack);

    radians = percent * M_PI * 2.0;
    GLfloat x_extent = half + length * (GLfloat)sin(radians);
    GLfloat y_extent = half + length * (GLfloat)cos(radians);

    glBegin(GL_TRIANGLE_FAN);
    glColor3f((GLfloat)redFore, (GLfloat)greenFore, (GLfloat)blueFore);
    glVertex2f(half, half);

    switch ((int)(percent * 8.0))
    {
    case 7:
        glVertex2f(half, length);
        glVertex2f(length, length);
        glVertex2f(length, 0.0);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, length);
        break;
    case 6:
        glVertex2f(half, length);
        glVertex2f(length, length);
        glVertex2f(length, 0.0);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, half);
        break;
    case 5:
        glVertex2f(half, length);
        glVertex2f(length, length);
        glVertex2f(length, 0.0);
        glVertex2f(0.0, 0.0);
        break;
    case 4:
        glVertex2f(half, length);
        glVertex2f(length, length);
        glVertex2f(length, 0.0);
        glVertex2f(half, 0.0);
        break;
    case 3:
        glVertex2f(half, length);
        glVertex2f(length, length);
        glVertex2f(length, half);
        glVertex2f(length, 0.0);
        break;
    case 2:
        glVertex2f(half, length);
        glVertex2f(length, length);
        glVertex2f(length, half);
        break;
    case 1:
        glVertex2f(half, length);
        glVertex2f(length, length);
        break;
    case 0:
        glVertex2f(half, length);
        break;
    }
    glVertex2f(x_extent, y_extent);

    glEnd();

    glBegin(GL_LINES);
    glColor3f((GLfloat)0.6, (GLfloat)0.0, (GLfloat)0.0);
    glVertex2f(half, half);
    glVertex2f(x_extent, y_extent);
    glColor3f((GLfloat)1.0, (GLfloat)1.0, (GLfloat)1.0);
    glEnd();
}

/************************
 * DRAW BULLSEYE
 ************************/
void SkeetInterface::drawBullseye(double angle) const
{
    double distance = storage.dimensions.getX();
    GLfloat x = storage.dimensions.getX() - distance * (GLfloat)cos(angle);
    GLfloat y = distance * (GLfloat)sin(angle);

    glBegin(GL_LINES);
    glColor3f((GLfloat)0.6, (GLfloat)0.6, (GLfloat)0.6);

    glVertex2f(x - 10.0f, y);
    glVertex2f(x + 10.0f, y);

    glVertex2f(x, y - 10.0f);
    glVertex2f(x, y + 10.0f);

    glColor3f((GLfloat)0.2, (GLfloat)0.2, (GLfloat)0.2);
    glVertex2f((GLfloat)storage.dimensions.getX(), (GLfloat)0.0);
    glVertex2f(x, y);

    glEnd();
}

/************************
 * SKEET DRAW LEVEL
 ************************/
void SkeetInterface::drawLevel() const
{
    drawBackground(storage.time.level() * .1, 0.0, 0.0);

    if (storage.bullseye)
        drawBullseye(storage.gun.getAngle());

    storage.gun.display();

    for (auto& pts : storage.points)
        pts.show();
    for (auto effect : storage.effects)
        effect->render();
    for (auto bullet : storage.bullets)
        bullet->output();
    for (auto element : storage.birds)
        element->draw();

    drawText(Position(10, storage.dimensions.getY() - 30), storage.score.getText());
    drawText(Position(storage.dimensions.getX() / 2 - 30, storage.dimensions.getY() - 30), storage.time.getText());
    drawText(Position(storage.dimensions.getX() - 110, storage.dimensions.getY() - 30), storage.hitRatio.getText());
}

/************************
 * SKEET DRAW STATUS
 ************************/
void SkeetInterface::drawStatus() const
{
    ostringstream sout;
    if (storage.time.isGameOver())
    {
        drawText(Position(storage.dimensions.getX() / 2 - 30, storage.dimensions.getY() / 2 + 10),
            "Game Over");

        drawText(Position(storage.dimensions.getX() / 2 - 30, storage.dimensions.getY() / 2 - 10),
            storage.score.getText());
    }
    else
    {
        drawTimer(1.0 - storage.time.percentLeft(),
            (storage.time.level() - 0.0) * .1, 0.0, 0.0,
            (storage.time.level() - 1.0) * .1, 0.0, 0.0);

        sout << "Level " << storage.time.level()
            << " begins in " << storage.time.secondsLeft() << " seconds";
        drawText(Position(storage.dimensions.getX() / 2 - 110, storage.dimensions.getY() / 2 - 10),
            sout.str());
    }
}
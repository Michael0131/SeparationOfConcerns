#pragma once

#include "uiInteract.h"
#include "SkeetStorage.h"
#include "SkeetLogic.h"

/*************************************************************************
 * SKEET INTERFACE
 * Handles all input/output. No rules. No game state (state is in storage).
 *************************************************************************/
class SkeetInterface
{
public:
    SkeetInterface(Position& dimensions);

    // handle all user input
    void interact(const UserInput& ui);

    // move the gameplay by one unit of time
    void animate();

    // output everything on the screen
    void drawLevel()  const;
    void drawStatus() const;

    // is the game currently playing right now?
    bool isPlaying() const { return storage.time.isPlaying(); }

private:
    // draw helpers (output only)
    void drawBackground(double redBack, double greenBack, double blueBack) const;
    void drawTimer(double percent,
        double redFore, double greenFore, double blueFore,
        double redBack, double greenBack, double blueBack) const;
    void drawBullseye(double angle) const;

private:
    SkeetStorage storage;
    SkeetLogic logic;
};
#pragma once

#include <list>
#include "position.h"
#include "bird.h"
#include "bullet.h"
#include "effect.h"
#include "gunInterface.h"
#include "time.h"
#include "score.h"
#include "points.h"

/*************************************************************************
 * SKEET STORAGE
 * All game state lives here. No input, no output, no rules.
 *************************************************************************/
class SkeetStorage
{
public:
	// NOTE: original Skeet took Position& dimensions; we can take const Position&
	SkeetStorage(const Position& dimensions);

	// ---- Game state (ALL state is here) ----
	GunInterface gun;               // gun state (you already separated Gun in gun.h)
	std::list<Bird*>   birds;       // all birds
	std::list<Bullet*> bullets;     // all bullets
	std::list<Effect*> effects;     // fragments/effects
	std::list<Points>  points;      // score popups

	Time time;                      // timer/level tracking
	Score score;                    // score
	HitRatio hitRatio;              // hit ratio

	Position dimensions;            // screen size
	bool bullseye;                  // UI flag (still state)
};
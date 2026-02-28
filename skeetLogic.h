#pragma once

#include "SkeetStorage.h"

/*************************************************************************
 * SKEET LOGIC
 * All mechanics/rules here. No OpenGL. No UserInput.
 * Does not own state; it operates on SkeetStorage.
 *************************************************************************/
class SkeetLogic
{
public:
	void animate(SkeetStorage& storage);

private:
	void spawn(SkeetStorage& storage);

	// helper used only by logic
	static int randomInt(int min, int max);
};
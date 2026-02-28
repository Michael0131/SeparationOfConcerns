#include "SkeetLogic.h"
#include <cassert>
#include <cstdlib>  // rand()

/************************
 * SKEET LOGIC : ANIMATE
 * move the gameplay by one unit of time
 ************************/
void SkeetLogic::animate(SkeetStorage& storage)
{
    storage.time++;

    // if status, then do not move the game
    if (storage.time.isStatus())
    {
        // get rid of the bullets and the birds without changing the score
        storage.birds.clear();
        storage.bullets.clear();
        storage.effects.clear();
        storage.points.clear();
        return;
    }

    // spawn
    spawn(storage);

    // move the birds and the bullets
    for (auto element : storage.birds)
    {
        element->advance();
        storage.hitRatio.adjust(element->isDead() ? -1 : 0);
    }

    for (auto bullet : storage.bullets)
        bullet->move(storage.effects);

    for (auto effect : storage.effects)
        effect->fly();

    for (auto& pts : storage.points)
        pts.update();

    // hit detection
    for (auto element : storage.birds)
        for (auto bullet : storage.bullets)
            if (!element->isDead() && !bullet->isDead() &&
                element->getRadius() + bullet->getRadius() >
                minimumDistance(element->getPosition(), element->getVelocity(),
                    bullet->getPosition(), bullet->getVelocity()))
            {
                for (int i = 0; i < 25; i++)
                    storage.effects.push_back(new Fragment(bullet->getPosition(), bullet->getVelocity()));
                element->kill();
                bullet->kill();
                storage.hitRatio.adjust(1);
                bullet->setValue(-(element->getPoints()));
                element->setPoints(0);
            }

    // remove the zombie birds
    for (auto it = storage.birds.begin(); it != storage.birds.end();)
        if ((*it)->isDead())
        {
            if ((*it)->getPoints())
                storage.points.push_back(Points((*it)->getPosition(), (*it)->getPoints()));
            storage.score.adjust((*it)->getPoints());
            it = storage.birds.erase(it);
        }
        else
            ++it;

    // remove zombie bullets
    for (auto it = storage.bullets.begin(); it != storage.bullets.end();)
        if ((*it)->isDead())
        {
            (*it)->death(storage.bullets);
            int value = -(*it)->getValue();
            storage.points.push_back(Points((*it)->getPosition(), value));
            storage.score.adjust(value);
            it = storage.bullets.erase(it);
        }
        else
            ++it;

    // remove zombie fragments
    for (auto it = storage.effects.begin(); it != storage.effects.end();)
        if ((*it)->isDead())
            it = storage.effects.erase(it);
        else
            ++it;

    // remove expired points
    for (auto it = storage.points.begin(); it != storage.points.end();)
        if ((*it).isDead())
            it = storage.points.erase(it);
        else
            ++it;
}

/******************************************************************
 * RANDOM INT
 * Generates a random integer between min and max inclusive-ish
 * (matches your original implementation)
 ****************************************************************/
int SkeetLogic::randomInt(int min, int max)
{
    assert(min < max);
    int num = (rand() % (max - min)) + min;
    assert(min <= num && num <= max);
    return num;
}

/************************
 * SKEET LOGIC : SPAWN
 * launch new birds
 ************************/
void SkeetLogic::spawn(SkeetStorage& storage)
{
    double size;
    switch (storage.time.level())
    {
        // in level 1 spawn big birds occasionally
    case 1:
        size = 30.0;
        // spawns when there is nothing on the screen
        if (storage.birds.size() == 0 && randomInt(0, 15) == 1)
            storage.birds.push_back(new Standard(size, 7.0));

        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Standard(size, 7.0));
        break;

        // two kinds of birds in level 2
    case 2:
        size = 25.0;
        // spawns when there is nothing on the screen
        if (storage.birds.size() == 0 && randomInt(0, 15) == 1)
            storage.birds.push_back(new Standard(size, 7.0, 12));

        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Standard(size, 5.0, 12));
        // spawn every 3 seconds
        if (randomInt(0, 3 * 30) == 1)
            storage.birds.push_back(new Sinker(size));
        break;

        // three kinds of birds in level 3
    case 3:
        size = 20.0;
        // spawns when there is nothing on the screen
        if (storage.birds.size() == 0 && randomInt(0, 15) == 1)
            storage.birds.push_back(new Standard(size, 5.0, 15));

        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Standard(size, 5.0, 15));
        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Sinker(size, 4.0, 22));
        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Floater(size));
        break;

        // three kinds of birds in level 4
    case 4:
        size = 15.0;
        // spawns when there is nothing on the screen
        if (storage.birds.size() == 0 && randomInt(0, 15) == 1)
            storage.birds.push_back(new Standard(size, 4.0, 18));

        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Standard(size, 4.0, 18));
        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Sinker(size, 3.5, 25));
        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Floater(size, 4.0, 25));
        // spawn every 4 seconds
        if (randomInt(0, 4 * 30) == 1)
            storage.birds.push_back(new Crazy(size));
        break;

    default:
        break;
    }
}
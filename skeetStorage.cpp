#include "SkeetStorage.h"

SkeetStorage::SkeetStorage(const Position& dimensions)
    : gun(Position(800.0, 0.0)),
    birds(),
    bullets(),
    effects(),
    points(),
    time(),
    score(),
    hitRatio(),
    dimensions(dimensions),
    bullseye(false)
{
}
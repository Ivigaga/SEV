#pragma once
#include "Weapon.h"
class Pistola :
    public Weapon
{
public:
    Pistola(float x, float y, Game* game);
    Projectile* shoot(int direction);
};



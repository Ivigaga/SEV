#pragma once
#include "Weapon.h"
class Escopeta :
    public Weapon
{
public:
    Escopeta(float x, float y, Game* game);

    Projectile* shoot(int direction);
};


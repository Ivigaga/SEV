#pragma once
#include "Enemy.h"
class BigEnemy :
    public Enemy
{
public:
    BigEnemy(float x, float y, Game* game);
    Projectile* attack();
    void update();
    Animation* aIdleRight;
    Animation* aIdleLeft;
};


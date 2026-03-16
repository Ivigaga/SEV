#pragma once
#include "Enemy.h"
class SmallEnemy :
    public Enemy
{
public:
    SmallEnemy(float x, float y, Game* game);
    Projectile* attack();
    void update();
    Animation* aAttackRight;
    Animation* aAttackLeft;
    Animation* aIdleRight;
    Animation* aIdleLeft;
    int shootTime = 95;
    int time = 0;
    int chargeTime = 6;
    int charge = 0;
    bool charging=false;
    Projectile* projectile;
    void restart();
};


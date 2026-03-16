#pragma once

#include "Actor.h"
#include "Audio.h"
#include "Projectile.h" 

class Weapon : public Actor
{
public:
	Weapon(int knockback, int munition,int recharge,float x, float y, Game* game);
	void update();
	virtual Projectile* shoot(int direction);
	Audio* audioShoot;
	Audio* audioRecharge;
	bool canShoot();
	void recharge();
	int retroceso;
	int amo;
	int bullets;
	int shootTime;
	int cadence;
	int timeForBullet=90;
	int rechargeTime;
	int isFull = true;
	enum Direcciones
	{
		ARRIBA, ABAJO, IZQUIERDA, DERECHA
	};
};


#include "Weapon.h"

Weapon::Weapon(int knockback, int munition, int recharge, float x, float y, Game* game) 
	:Actor("res/shotgun_right.png", x, y, 32, 14, game)
{
	retroceso = knockback;
	amo = munition;
	bullets = munition;
	shootTime = recharge;
	cadence = shootTime;
	audioRecharge = new Audio("res/gun-load.wav", false);
}

void Weapon::update() {
	shootTime++;
	if (shootTime > cadence) {
		shootTime = cadence;
	}
	recharge();
}

Projectile* Weapon::shoot(int direction) {
	Projectile* projectile = new Projectile("res/disparo_jugador2.png", x, y, 20, 20, game);
	if(canShoot()){
		switch (direction)
		{
		case ARRIBA:
			projectile->vx = 0;
			projectile->vy = -9;
			break;
		case ABAJO:
			projectile->vx = 0;
			projectile->vy = 9;
			break;
		case DERECHA:
			projectile->vx = 9;
			projectile->vy = 0;
			break;
		case IZQUIERDA:
			projectile->vx = -9;
			projectile->vy = 0;
			break;
		default:
			projectile->vx = 9;
			projectile->vy = 0;
			break;
		}
		bullets--;
		shootTime = 0;
		isFull = false;
		return projectile;
	}
	else {
		return NULL;
	}


}

bool Weapon::canShoot() {
	return (bullets > 0 && shootTime>=cadence);
}

void Weapon::recharge() {
	if (!isFull && bullets < amo) {
		rechargeTime++;
		if (rechargeTime >= timeForBullet) {
			rechargeTime = 0;
			bullets++;
			audioRecharge->play();
			if (bullets >= amo) {
				isFull = true;
			}
		}
	}
	else {
		if (rechargeTime > 0) {
			rechargeTime = 0;
		}
	}
}
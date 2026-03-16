#include "Escopeta.h"


Escopeta::Escopeta(float x, float y, Game* game)
	:Weapon(14, 2, 20, x, y, game) {
	audioShoot = new Audio("res/shotgun_shot.wav", false);
}

Projectile* Escopeta::shoot(int direction) {
	Projectile* projectile;
	if (canShoot()) {
		if (bullets > 2) {
			int a = 2;
		}
		switch (direction)
		{
		case ARRIBA:
			projectile = new Projectile(0,-9,3,true,8,"res/disparo_jugador2.png", x, y-height/2,60,20, game);
			break;
		case ABAJO:
			projectile = new Projectile(0, 9, 3, true, 8, "res/disparo_jugador2.png", x, y, 60, 20, game);
			break;
		case DERECHA:
			projectile = new Projectile(9, 0, 3, true, 8, "res/disparo_jugador2.png", x, y, 20, 60, game);
			break;
		case IZQUIERDA:
			projectile = new Projectile(-9, 0, 3, true, 8, "res/disparo_jugador2.png", x, y, 20, 60, game);
			break;
		default:
			projectile = new Projectile(9, 0, 3, true, 8, "res/disparo_jugador2.png", x, y, 20, 60, game);
			break;
		}
		bullets--;
		shootTime = 0;
		isFull = false;
		audioShoot->play();
		return projectile;
	}
	else {
		return NULL;
	}


}
#include "Pistola.h"


Pistola::Pistola(float x, float y, Game* game)
	:Weapon(8,4,12,x,y,game)
{
	audioShoot = new Audio("res/pistol-shot.wav", false);
}
Projectile* Pistola::shoot(int direction) {
	Projectile* projectile;
	if (canShoot()) {
		switch (direction)
		{
		case ARRIBA:
			projectile = new Projectile(0,-9,1,"res/disparo_jugador2.png",x,y,20,20,game);
			break;
		case ABAJO:
			projectile = new Projectile(0, 9, 1, "res/disparo_jugador2.png", x, y, 20, 20, game);
			break;
		case DERECHA:
			projectile = new Projectile(9, 0, 1, "res/disparo_jugador2.png", x, y, 20, 20, game);
			break;
		case IZQUIERDA:
			projectile = new Projectile(-9, 0, 1, "res/disparo_jugador2.png", x, y, 20, 20, game);
			break;
		default:
			projectile = new Projectile(9, 0, 1, "res/disparo_jugador2.png", x, y, 20, 20, game);
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




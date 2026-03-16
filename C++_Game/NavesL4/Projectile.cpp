#include "Projectile.h"

Projectile::Projectile(string fileName, float x, float y, int width, int height, Game* game) :
	Actor(fileName, x, y, width, height, game) {
	vx = 9;
	vy = -1; // La gravedad inicial es 1
}

Projectile::Projectile(int vx, int vy, int damage, string fileName, float x, float y, int width, int height, Game* game) :
	Actor(fileName, x, y, width, height, game) {
	this->vx = vx;
	this->vy = vy;
	this->damage = damage;
}

Projectile::Projectile(int vx, int vy, int damage, bool life,int lifeSpam, string fileName, float x, float y, int width, int height, Game* game) :
	Actor(fileName, x, y, width, height, game) {
	this->vx = vx;
	this->vy = vy;
	this->damage = damage;
	timeDependant = life;
	this->lifeSpam = lifeSpam;
}

void Projectile::update() {
	vy = vy - 1; // La gravedad suma 1 en cada actualización restamos para anularla vy = vy - 1; // La gravedad suma 1 en cada actualización restamos para anularla
	if (lastFrameX == x && lastFrameY == y) {
		isNotMoving = true;
	}
	lastFrameX = x;
	lastFrameY = y;
	timePassed++;
	if (timePassed > lifeSpam) {
		timePassed = lifeSpam;
	}
}

bool Projectile::shouldDie() {
	if (isNotMoving)
		return true;
	if (timeDependant) {
		return timePassed >= lifeSpam;
	}
	return false;
}


#include "Enemy.h"

Enemy::Enemy(string filename,int life,float x, float y,int width,int height, Game* game)
	: Actor(filename, x, y, width, height, game) {

	aDying = new Animation("res/enemigo_morir.png", width, height,
		280, 40, 6, 8, false, game);

	aMovingRight = new Animation("res/enemyS_walk_right.png", width, height,
		56, 30, 6, 2, true, game);

	aMovingLeft = new Animation("res/enemyS_walk_left.png", width, height,
		56, 30, 6, 2, true, game);

	state = game->stateMoving;


	animation = aMovingRight;

	vx = 3;
	vxIntelligence = 1;
	vx = vxIntelligence;
	this->life = life;
	currentLife = life;

}

void Enemy::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}


	if (state == game->stateMoving) {
		if (vx > 0) {
			animation = aMovingRight;
		}
		else {
			animation = aMovingLeft;
		}

	}
	if (state == game->stateDying) {
		animation = aDying;
	}

	// Establecer velocidad
	if (state != game->stateDying) {
		// no está muerto y se ha quedado parado
		if (vx == 0) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
		if (outRight) {
			// mover hacia la izquierda vx tiene que ser negativa
			if (vxIntelligence > 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}
		if (outLeft) {
			// mover hacia la derecha vx tiene que ser positiva
			if (vxIntelligence < 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}

	}
	else {
		vx = 0;
	}



}

void Enemy::impacted(int damage) {
	if (state != game->stateDying) {
		if (damage >= currentLife || currentLife<=0) {
			state = game->stateDying;
		}
		else {
			currentLife -= damage;
			if (!seesPlayer(player)) {
				vxIntelligence = vxIntelligence * -1;
				vx = vxIntelligence;
			}
		}
		
		if (currentLife <= 0) {
			state = game->stateDying;
		}
	}
}


void Enemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y + scrollY);
}


Projectile* Enemy::attack() {
	return NULL;
}

bool Enemy::seesPlayer(Player* player) {

	this->player = player;
	if (player != NULL) {
		if (abs(player->x - x) < 250 && abs(player->y - y) < 250)
			if (player->x < x) {
				playerAt = IZQUIERDA;
				if(abs(player->x - x) < abs(player->y - y) && player->y < y){
					playerAt = ARRIBA;
				}
				if (player->y > y + 30 ) {
					return false;
				}
				return orientation == game->orientationLeft;
			}
			else {
				playerAt = DERECHA;
				if (abs(player->x - x) < abs(player->y - y) && player->y < y) {
					playerAt = ARRIBA;
				}
				if (player->y > y + 30) {
					return false;
				}
				return orientation == game->orientationRight;
			}
	}
	
	return false;
	
		
}

void Enemy::restart() {
	x = startX;
	y = startY;
	state = game->stateMoving;
	currentLife = life;
	orientation = game->orientationLeft;
	playerAt = IZQUIERDA;
	vxIntelligence = 1;
	vx = vxIntelligence;
}
#include "BigEnemy.h"
BigEnemy::BigEnemy(float x, float y, Game* game)
	:Enemy("res/enemyB.png", 3, x, y, 84, 69, game) {
	aDyingRight = new Animation("res/enemyB_dead_right.png", width, height,
		416, 46, 2, 8, false, game);
	aDyingLeft = new Animation("res/enemyB_dead_left.png", width, height,
		416, 46, 2, 8, false, game);
	aIdleRight = new Animation("res/enemyB_idle_right.png", width, height,
		364, 46, 3, 7, true, game);

	aIdleLeft = new Animation("res/enemyB_idle_left.png", width, height,
		364, 46, 3, 7, true, game);

	aMovingRight = new Animation("res/enemyB_walking_right.png", width, height,
		364, 46, 3, 7, true, game);

	aMovingLeft = new Animation("res/enemyB_walking_left.png", width, height,
		364, 46, 3, 7, true, game);
}

Projectile* BigEnemy::attack() {
	if (state != game->stateDying && state != game->stateDead)
	{
		if (seesPlayer(player))
		{
			float px = player->x;
			float py = player->y;


			if (playerAt == ARRIBA) {
				vx = 0;
			}
			else if (playerAt == IZQUIERDA) {
				vx -= 1;
				if (vx < -5) {
					vx = -5;
				}
			}
			else if(playerAt==DERECHA){
				vx += 1;
				if (vx > 5) {
					vx = 5;
				}
			}

		}
	}
	
	return NULL;
	
}

void BigEnemy::update() {

	if (state == game->stateMoving) {
		if (vx != 0)
		{

			if (orientation == game->orientationRight) {
				animation = aMovingRight;
			}
			else {
				animation = aMovingLeft;

			}
		}
		else {
			if (orientation == game->orientationRight) {
				animation = aIdleRight;
			}
			else {
				animation = aIdleLeft;
			}
		}

	}

	
	if (state == game->stateDying) {
		if (orientation == game->orientationLeft)
			animation = aDyingLeft;
		else
			animation = aDyingRight;
	}


	// Actualizar la animación
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
		else {
			state = game->stateMoving;
		}
	}

	// Establecer velocidad
	if (state != game->stateDying) {
		// no está muerto y se ha quedado parado
		if (vx == 0 && !seesPlayer(player)) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
		if (outRight) {
			// mover hacia la izquierda vx tiene que ser negativa
			if (vxIntelligence > 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
			if (seesPlayer(player) &&  playerAt==DERECHA ) {
				vx = 0;
			}
		}
		if (outLeft) {
			// mover hacia la derecha vx tiene que ser positiva
			if (vxIntelligence < 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
			if (seesPlayer(player) && playerAt==IZQUIERDA) {
				vx = 0;
			}
		}
		if (vx > 0) {
			orientation = game->orientationRight;
		}
		else if (vx < 0) {
			orientation = game->orientationLeft;
		}

	}
	else {

		vx = 0;

	}



}
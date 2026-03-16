#include "SmallEnemy.h"


SmallEnemy::SmallEnemy(float x, float y, Game* game)
	:Enemy("res/enemyS.png",1,x,y,28,30,game) {
	aDyingRight = new Animation("res/enemyL_dead_right.png", width, height,
		112, 30, 2,4, false, game);
	aDyingLeft = new Animation("res/enemyL_dead_left.png", width, height,
		112, 30, 2, 4, false, game);
	aIdleRight = new Animation("res/enemyS_right.png", width, height,
		width, height, 30, 1, true, game);

	aIdleLeft = new Animation("res/enemyS.png", width, height,
		width, height, 30, 1, true, game);

	aMovingRight = new Animation("res/enemyS_walk_right.png", width, height,
		56, 30, 4, 2, true, game);

	aMovingLeft = new Animation("res/enemyS_walk_left.png", width, height,
		56, 30, 4, 2, true, game);
	aAttackRight = new Animation("res/enemyS_attack_right.png", width, height,
		84, 30, 2, 3, false, game);
	aAttackLeft= new Animation("res/enemyS_attack_left.png", width, height,
		84, 30, 2, 3, false, game);
}

Projectile* SmallEnemy::attack() {
	if (state!=game->stateShooting && state!=game->stateDying && state!=game->stateDead && time>=shootTime && !charging)
	{
		float px = player->x;
		float py = player->y;
		aAttackLeft->currentFrame = 0; //"Rebobinar" aniamción
		aAttackRight->currentFrame = 0; //"Rebobinar" aniamción
		state = game->stateShooting;
		if (abs(px - x) <  abs(py - y) && py < y) {
			projectile = new Projectile(0, -9, 1, "res/disparo_jugador_vertical.png", x,y,6,18,game);
		}
		else {
			if (px < x) {
				orientation = game->orientationLeft;
				projectile = new Projectile(-9, 0, 1, "res/disparo_jugador.png", x, y, 18, 6, game);
			}
			else {
				orientation = game->orientationRight;
				projectile = new Projectile(9, 0, 1, "res/disparo_jugador.png", x, y, 18, 6, game);
			}
		}
		time = 0;
		charging = true;
		return NULL;
	}
	else if (charging) {
		if (charge >= chargeTime) {
			charging = false;
			charge = 0;
			return projectile;
		}else{
			return NULL;
		}
	}
	else {
		return NULL;
	}
}

void SmallEnemy::update() {
	
	time++;
	if (charging) {
		charge++;
		if (charge > chargeTime) {
			charge = chargeTime;
		}
	}
	if (state == game->stateMoving) {
		if (vx!=0)
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

	if (state == game->stateShooting) {
		vx = 0;
		if (orientation == game->orientationRight) {
			animation = aAttackRight;
		}
		else {
			animation = aAttackLeft;
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
	if (state != game->stateDying && state!=game->stateShooting && !seesPlayer(player)) {
		// no está muerto y se ha quedado parado
		if (vx == 0 ) {
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

void SmallEnemy::restart() {
	x = startX;
	y = startY;
	state = game->stateMoving;
	currentLife = life;
	orientation = game->orientationLeft;
	time = 0;
	charge = 0;
	charging = false;
	projectile=NULL;
	playerAt = IZQUIERDA;
	vxIntelligence = 1;
	vx = vxIntelligence;
}
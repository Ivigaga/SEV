#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 32, 43, game) {
	onAir = false;
	orientation = game->orientationRight;
	state = game->stateMoving;
	aShootingRight = new Animation("res/jugador_disparando_derecha.png",
		width, height, 160, 40, 6, 4, false, game);
	aShootingLeft = new Animation("res/jugador_disparando_izquierda.png",
		width, height, 160, 40, 6, 4, false, game);
	aJumpingRight = new Animation("res/air_aim_right.png",
		32, 43, 32, 43, 30, 1, true, game);
	aJumpingLeft = new Animation("res/air_aim_left.png",
		32, 43, 32, 43, 30, 1, true, game);
	aJumpingRightBack = new Animation("res/air_aim_right_back.png",
		36, 43, 36, 43, 30, 1, true, game);
	aJumpingLeftBack = new Animation("res/air_aim_left_back.png",
		36, 43, 36, 43, 30, 1, true, game);
	aJumpingRightUp = new Animation("res/air_aim_right_up.png",
		32, 43, 32, 43, 30, 1, true, game);
	aJumpingLeftUp = new Animation("res/air_aim_left_up.png",
		32, 43, 32, 43, 30, 1, true, game);
	aJumpingRightDown = new Animation("res/air_aim_down.png",
		32, 43, 32, 43, 30, 1, true, game);
	aJumpingLeftDown= new Animation("res/air_aim_down_left.png",
		32, 43, 32, 43, 30, 1, true, game);

	aIdleRight = new Animation("res/jugador.png", width, height,
		32, 43, 30, 1, true, game);
	aIdleLeft = new Animation("res/jugador_left.png", width, height,
		32, 43, 30, 1, true, game);
	aIdleRightUp = new Animation("res/player_aim_up.png", width, height,
		32, 43, 30, 1, true, game);
	aIdleLeftUp= new Animation("res/player_aim_up_left.png", width, height,
		32, 43, 30, 1, true, game);
	aIdleRightDown = new Animation("res/aim_down.png", width, height,
		32, 43, 30, 1, true, game);
	aIdleLeftDown = new Animation("res/aim_down_left.png", width, height,
		32, 43, 30, 1, true, game);
	aRunningRight = new Animation("res/running_aim_right_3.png", 32, 43,
		384, 43, 2, 12, true, game);
	aRunningLeft = new Animation("res/running_aim_left_3.png", 32, 43,
		384, 43, 2, 12, true, game);
	aRollRight = new Animation("res/roll_right.png", 32, 24,
		128, 24, 3, 4, false, game);
	aRollLeft = new Animation("res/roll_left.png", 32, 24,
		128, 24, 3, 4, false, game);

	aAimBack = new Animation("res/aim_back.png", width, height,
		33, 43, 30, 1, true, game);
	aAimBackLeft = new Animation("res/aim_back_left.png", width, height,
		33, 43, 30, 1, true, game);

	aRunAimBack = new Animation("res/running_aim_right_back.png", 36, 43,
		432, 43, 2, 12, true, game);
	aRunAimBackLeft = new Animation("res/running_aim_left_back.png", 36, 43,
		432, 43, 2, 12, true, game);
	aRunAimUp = new Animation("res/runing_right_up.png", 32, 43,
		384, 43, 2, 12, true, game);
	aRunAimUpLeft = new Animation("res/runing_left_up.png", 32, 43,
		384, 43, 2, 12, true, game);
	aRunAimDown = new Animation("res/running_aim_right_down.png", 32, 43,
		384, 43, 2, 12, true, game);
	aRunAimDownLeft = new Animation("res/running_aim_left_down.png", 32, 43,
		384, 43, 2, 12, true, game);

	animation = aIdleRight;
	pistola = new Pistola(x + width / 2, y - height / 2, game);
	escopeta = new Escopeta(x + width / 2, y - height / 2, game);
	weapon = escopeta;
	audioJump = new Audio("res/jump.wav", false);
}

void Player::update() {
	
	// En el aire y moviéndose, PASA a estar saltando
	if (onAir && state == game->stateMoving) {
		state = game->stateJumping;
	}
	vx *= 0.9;
	if (dashing) {
		state = game->stateDashing;
		vx = maxSpeed;
		if (orientation == game->orientationLeft) {
			vx = -maxSpeed;
		}
		dashCount++;
		if (dashCount >= dashTime) {
			dashCount = 0;
			dashing = false;
			vx = vx / 2;
			dashRecharge = 0;
		}
	}
	dashRecharge++;
	if (dashRecharge >= 15) {
		dashRecharge = 15;
	}
	// No está en el aire y estaba saltando, PASA a moverse
	if (!dashing && state == game->stateDashing) {
		if (onAir) {
			state = game->stateJumping;
		}
		else {
			state = game->stateMoving;
		}
	}
	if ((!onAir && state == game->stateJumping)) {
		state = game->stateMoving;
		
	}
	if (!onAir) {
		shootOnAir = false;
	}
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}
	// Selección de animación basada en estados
	if (state == game->stateJumping ) {
		if (orientation == game->orientationRight) {
			//animation = aJumpingRight;
			switch (aimingAt)
			{
			case ARRIBA:
				animation = aJumpingRightUp;
				break;
			case ABAJO:
				animation = aJumpingRightDown;
				break;
			case DERECHA:
				animation = aJumpingRight;
				break;
			case IZQUIERDA:
				animation = aJumpingRightBack;
				break;
			default:
				switch (direccion)
				{
				case ARRIBA:
					animation = aJumpingRightUp;
					break;
				case ABAJO:
					animation = aJumpingRightDown;
					break;
				case DERECHA:
					animation = aJumpingRight;
					break;
				case IZQUIERDA:
					animation = aJumpingRightBack;
					break;
				default:
					animation = aJumpingRight;
					break;
				}
				break;
			}
		}
		if (orientation == game->orientationLeft) {
			switch (aimingAt)
			{
			case ARRIBA:
				animation = aJumpingLeftUp;
				break;
			case ABAJO:
				animation = aJumpingLeftDown;
				break;
			case DERECHA:
				animation = aJumpingLeftBack;
				break;
			case IZQUIERDA:
				animation = aJumpingLeft;
				break;
			default:
				switch (direccion)
				{
				case ARRIBA:
					animation = aJumpingLeftUp;
					break;
				case ABAJO:
					animation = aJumpingLeftDown;
					break;
				case DERECHA:
					animation = aJumpingLeftBack;
					break;
				case IZQUIERDA:
					animation = aJumpingLeft;
					break;
				default:
				
					animation = aJumpingLeft;
					break;
				}
				break;
			}
		}
	}

	if (state == game->stateDashing) {
		if (orientation == game->orientationRight) {
			animation = aRollRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aRollLeft;
		}
	}
	bool endAnimation = animation->update();
	if (collisionDown == true) {
		onAir = false;
	}
	else {
		onAir = true;
	}
	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == game->stateShooting || state==game->stateDashing) {
			state = game->stateMoving;
		}
	}


	if (!shootOnAir) {

		if (vx > 0) {
			orientation = game->orientationRight;
		}
		if (vx < 0) {
			orientation = game->orientationLeft;
		}
	}

	// Selección de animación basada en estados
	/*
	if (state == game->stateShooting) {
		if (orientation == game->orientationRight) {
			animation = aShootingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aShootingLeft;
		}
	}
	*/
	if (state == game->stateMoving) {
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				switch (aimingAt)
				{
				case ARRIBA:
					animation = aRunAimUp;
					break;
				case ABAJO:
					animation = aRunAimDown;
					break;
				case DERECHA:
					animation = aRunningRight;
					break;
				case IZQUIERDA:
					animation = aRunAimBack;
					break;
				default:
					switch (direccion)
					{
					case ARRIBA:
						animation = aRunAimUp;
						break;
					case ABAJO:
						animation = aRunAimDown;
						break;
					case DERECHA:
						animation = aRunningRight;
						break;
					case IZQUIERDA:
						animation = aRunAimBack;
						break;
					default:
						animation = aRunningRight;
						break;
					}
					break;
				}
				
			}
			if (orientation == game->orientationLeft) {

				switch (aimingAt)
				{
				case ARRIBA:
					animation = aRunAimUpLeft;
					break;
				case ABAJO:
					animation = aRunAimDownLeft;
					break;
				case DERECHA:
					animation = aRunAimBackLeft;
					break;
				case IZQUIERDA:
					animation = aRunningLeft;
					break;
				default:
					switch (direccion)
					{
					case ARRIBA:
						animation = aRunAimUpLeft;
						break;
					case ABAJO:
						animation = aRunAimDownLeft;
						break;
					case DERECHA:
						animation = aRunAimBackLeft;
						break;
					case IZQUIERDA:
						animation = aRunningLeft;
						break;
					default:
						animation = aRunningLeft;
						break;
					}
					break;
				}
				
			}
		}
		if (vx == 0) {
			if (orientation == game->orientationRight) {
				switch (aimingAt)
				{
				case ARRIBA:
					animation = aIdleRightUp;
					break;
				case ABAJO:
					animation = aIdleRightDown;
					break;
				case DERECHA:
					animation = aIdleRight;
					break;
				case IZQUIERDA:
					animation = aAimBack;
					break;
				default:
					switch (direccion)
					{
					case ARRIBA:
						animation = aIdleRightUp;
						break;
					case ABAJO:
						animation = aIdleRightDown;
						break;
					case DERECHA:
						animation = aIdleRight;
						break;
					case IZQUIERDA:
						animation = aAimBack;
						break;
					default:
						animation = aIdleRight;
						break;
					}
					break;
				}
				
			}
			if (orientation == game->orientationLeft) {
				switch (aimingAt)
				{
				case ARRIBA:
					animation = aIdleLeftUp;
					break;
				case ABAJO:
					animation = aIdleLeftDown;
					break;
				case DERECHA:
					animation = aAimBackLeft;
					break;
				case IZQUIERDA:
					animation = aIdleLeft;
					break;
				default:
					switch (direccion)
					{
					case ARRIBA:
						animation = aIdleLeftUp;
						break;
					case ABAJO:
						animation = aIdleLeftDown;
						break;
					case DERECHA:
						animation = aAimBackLeft;
						break;
					case IZQUIERDA:
						animation = aIdleLeft;
						break;
					default:
						animation = aIdleLeft;
						break;
					}
					break;
				}
				
			}
		}
	}


	if (shootTime > 0) {
		shootTime--;
	}
	
	switch (aimingAt)
	{
	case ARRIBA:
		if (orientation == game->orientationLeft) {
			escopeta->texture = game->getTexture("res/shotgun_left.png");
			pistola->texture = game->getTexture("res/shotgun_left.png");
		}
		else {
			escopeta->texture = game->getTexture("res/shotgun_right.png");
			pistola->texture = game->getTexture("res/shotgun_right.png");
		}
		escopeta->x = x;
		escopeta->y = y-height/2-escopeta->height /2+2;
		pistola->x = x;
		pistola->y = y - height / 2 - escopeta->height / 2;
		break;
	case ABAJO:
		escopeta->x = x;
		escopeta->y = y + height / 2;
		pistola->x = x;
		pistola->y = y + height / 2;
		break;
	case DERECHA:
		escopeta->x = x + width/2;
		escopeta->y = y;
		pistola->x = x + width / 2;
		pistola->y = y;
		break;
	case IZQUIERDA:
		escopeta->x = x - width / 2;
		escopeta->y = y;
		pistola->x = x - width / 2;
		pistola->y = y;
		break;
	default:
		switch (direccion)
		{
		case ARRIBA:
			if (orientation == game->orientationLeft) {
				escopeta->texture = game->getTexture("res/shotgun_left.png");
				pistola->texture = game->getTexture("res/shotgun_left.png");
			}
			else {
				escopeta->texture = game->getTexture("res/shotgun_right.png");
				pistola->texture = game->getTexture("res/shotgun_right.png");
			}
			escopeta->x = x;
			escopeta->y = y - height / 2 - escopeta->height / 2;
			pistola->x = x;
			pistola->y = y - height / 2 - escopeta->height / 2;
			break;
		case ABAJO:
			escopeta->x = x;
			escopeta->y = y + height / 2;
			pistola->x = x;
			pistola->y = y + height / 2;
			break;
		case DERECHA:
			escopeta->x = x + width / 2;
			escopeta->y = y;
			pistola->x = x + width / 2;
			pistola->y = y;
			break;
		case IZQUIERDA:
			escopeta->x = x - width / 2;
			escopeta->y = y;
			pistola->x = x - width / 2;
			pistola->y = y;
			break;
		default:
			if (orientation == game->orientationLeft) {
				escopeta->x = x - width / 2;
				escopeta->y = y;
				pistola->x = x - width / 2;
				pistola->y = y;
			}
			else {
				escopeta->x = x + width / 2;
				escopeta->y = y;
				pistola->x = x + width / 2;
				pistola->y = y;
			}
			break;
		}
		break;
	}
	pistola->update();
	escopeta->update();
	changeCooldown++;
	if (changeCooldown > 20) {
		changeCooldown = 20;
	}
}

void Player::moveX(float axis) {
	if (!shootOnAir) {
		vx += axis * 1.5;
		if (vx >= maxSpeed) {
			vx *= 0.75;
			cout << "vx=" << vx << endl;
		}
		else if (vx <= -maxSpeed) {
			vx *= 0.75;
		}
	}
	cout << "vx=" << vx << endl;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {

	if (weapon->canShoot() && !dashing) {
		//state = game->stateShooting;
		aShootingLeft->currentFrame = 0; //"Rebobinar" aniamción
		aShootingRight->currentFrame = 0; //"Rebobinar" aniamción
		shootTime = shootCadence;
		return aim();

	}
	else {
		return NULL;
	}
}

void Player::draw(float scrollX, float scrollY) {

	animation->draw(x - scrollX, y + scrollY);
	
	
}
void Player::jump() {
	if (!onAir) {
		audioJump->play();
		dashing = false;
		vy = -12;
		onAir = true;
	}
}

void Player::loseLife() {
	if (lifes > 0) {
		lifes--;
	}
}

Projectile* Player::aim() {
	Projectile* p = NULL;
	switch (aimingAt)
	{
	case ARRIBA:
		p = weapon->shoot(ARRIBA);
		if (p != NULL) {
			retroceso(ARRIBA, weapon->retroceso);
		}
		break;
	case ABAJO:
		p = weapon->shoot(ABAJO);
		if (p != NULL) {
			retroceso(ABAJO, weapon->retroceso);
		}
		break;
	case DERECHA:
		p = weapon->shoot(DERECHA);
		if (p != NULL) {
			retroceso(DERECHA, weapon->retroceso);
		}
		break;
	case IZQUIERDA:
		p = weapon->shoot(IZQUIERDA);
		if (p != NULL) {
			retroceso(IZQUIERDA, weapon->retroceso);
		}
		break;
	default:
		switch (direccion)
		{
		case ARRIBA:
			p = weapon->shoot(ARRIBA);
			if (p != NULL) {
				retroceso(ARRIBA, weapon->retroceso);
			}
			break;
		case ABAJO:
			p = weapon->shoot(ABAJO);
			if (p != NULL) {
				retroceso(ABAJO, weapon->retroceso);
			}
			break;
		case DERECHA:
			p = weapon->shoot(DERECHA);
			if (p != NULL) {
				retroceso(DERECHA, weapon->retroceso);
			}
			break;
		case IZQUIERDA:
			p = weapon->shoot(IZQUIERDA);
			if (p != NULL) {
				retroceso(IZQUIERDA, weapon->retroceso);
			}
			break;
		default:
			if (orientation == game->orientationLeft) {
				p = weapon->shoot(IZQUIERDA);
				if (p != NULL) {
					retroceso(IZQUIERDA, weapon->retroceso);
				}
			}
			else {
				p = weapon->shoot(DERECHA);
				if (p != NULL) {
					retroceso(DERECHA, weapon->retroceso);
				}
			}
			break;
		}
		break;
	}
	return p;

}



void Player::retroceso(int axis, int knockBack) {

	if (knockBack>0)
	{
		switch (axis)
		{
		case ARRIBA:
			vy = knockBack;
			break;
		case ABAJO:
			vy = -knockBack;
			break;
		case DERECHA:
			if (onAir) {
				vx -= knockBack;
				shootOnAir = true;
			}
			else {
				vx = 0;
			}
			break;
		case IZQUIERDA:
			if (onAir) {
				vx += knockBack;
				shootOnAir = true;
			}
			else {
				vx = 0;
			}
			break;
		default:
			break;
		}
	}
}

void Player::dash() {
	if (!onAir && !dashing && dashRecharge==15) {
		aRollLeft->currentFrame = 0; //"Rebobinar" aniamción
		aRollRight->currentFrame = 0; //"Rebobinar" aniamción
		vx = maxSpeed;
		if (orientation == game->orientationLeft) {
			vx = -maxSpeed;
		}
		dashing = true;
	}
}

void Player::changeWeapon(){
	if (changeCooldown>=20)
	{
		if (weapon == escopeta) {
			weapon = pistola;
		}
		else {
			weapon = escopeta;
		}
		changeCooldown = 0;
	}
}


void Player::restart() {
	x = startX;
	y = startY;
	vx = 0;
	vy=0;
	lifes = 1;
	escopeta->bullets = escopeta->amo;
	escopeta->rechargeTime = 0;
	escopeta->shootTime = escopeta->cadence;
	pistola->bullets = pistola->amo;
	pistola->rechargeTime = 0;
	pistola->shootTime = pistola->cadence;
}
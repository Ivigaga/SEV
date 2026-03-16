#pragma once

#include "Actor.h"
#include "Audio.h"
#include "Projectile.h" 
#include "Weapon.h"
#include "Pistola.h" 
#include "Escopeta.h" 
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	void update();
	void jump();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY=0) override; // Va a sobrescribir
	void loseLife();
	int lifes = 1;
	int invulnerableTime = 0;
	bool onAir;
	int orientation;
	int state;
	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aJumpingRight;
	Animation* aJumpingLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aShootingRight;
	Animation* aShootingLeft;
	Animation* animation; // Referencia a la animación 
	int shootCadence = 30;
	int shootTime = 0;

	bool shootOnAir = false;
	enum Direcciones
	{
		ARRIBA, ABAJO, IZQUIERDA, DERECHA,NEUTRAL
	};
	int direccion=NEUTRAL;


	int aimingAt=NEUTRAL;

	Projectile* aim();
	void retroceso(int axis, int knockBack);
	void dash();
	void changeWeapon();
	bool dashing=false;
	int dashTime = 12;
	int dashCount = 0;
	int dashRecharge = 15;
	int maxSpeed=12;

	Weapon* weapon;
	Pistola* pistola;
	Escopeta* escopeta;
	int changeCooldown = 20;

	Animation* aRollRight;
	Animation* aRollLeft;
	Animation* aAimBack;
	Animation* aAimBackLeft;
	Animation* aRunAimBack;
	Animation* aRunAimBackLeft;
	Animation* aRunAimUp;
	Animation* aRunAimUpLeft;
	Animation* aJumpingRightUp;
	Animation* aJumpingLeftUp;
	Animation* aIdleRightUp;
	Animation* aIdleLeftUp;

	Animation* aRunAimDown;
	Animation* aRunAimDownLeft;
	Animation* aJumpingRightDown;
	Animation* aJumpingLeftDown;
	Animation* aIdleRightDown;
	Animation* aIdleLeftDown;

	Animation* aJumpingRightBack;
	Animation* aJumpingLeftBack;
	Audio* audioJump;
	void restart();
};


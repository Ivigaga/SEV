#pragma once

#include "Actor.h"
#include "Animation.h" 
#include "Player.h"

class Enemy : public Actor
{
public:
	Enemy(string fileName,int life,float x, float y, int width, int height, Game* game);
	void draw(float scrollX = 0, float scrollY=0) override; // Va a sobrescribir
	virtual void update();
	void impacted(int damage); // Recibe impacto y pone animación de morir
	float vxIntelligence;
	int state;
	Animation* aDying;
	Animation* aDyingLeft;
	Animation* aDyingRight;
	Animation* aMovingRight;
	Animation* aMovingLeft;
	Animation* animation; // Referencia a la animación mostrada

	virtual Projectile* attack();
	bool seesPlayer(Player* player);
	int life;
	int currentLife;
	Player* player;
	int orientation=game->orientationLeft;

	enum positionPlayer {
		ARRIBA,DERECHA,IZQUIERDA
	};
	int playerAt=IZQUIERDA;
	virtual void restart();
};

#pragma once

#include "Actor.h"


class Projectile : public Actor
{
public:
	Projectile(string fileName,float x, float y,int width,int height, Game* game);
	Projectile(int vx, int vy, int damage, string fileName, float x, float y, int width, int height, Game* game);
	Projectile(int vx, int vy, int damage, bool life,int lifeSpam, string fileName, float x, float y, int width, int height, Game* game);
	void update();
	bool shouldDie();
	bool isNotMoving=false;
	float lastFrameX;
	float lastFrameY;
	int lifeSpam = 0;
	int timePassed=0;
	bool timeDependant=false;
	int damage = 1;
};


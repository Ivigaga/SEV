#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "Projectile.h"
#include "Tile.h"
#include "Pad.h"
#include "Text.h"
#include "Space.h" // importar
#include "Audio.h" 

#include "SmallEnemy.h"
#include "BigEnemy.h"
#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void gamePadToControls(SDL_Event event); // USO DE GAMEPAD
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	Actor* message;
	bool pause;
	// Elementos de interfaz
	SDL_GameController* gamePad;
	//Pad* pad;
	//Actor* buttonJump;
	//Actor* buttonShoot;
	Tile* cup; // Elemento de final de nivel
	Space* space;
	float scrollX;
	int mapWidth;

	//Ext-Scroll
	float scrollY;
	int mapHeight;

	list<Tile*> tiles;

	Audio* audioBackground;
	//Text* textPoints;
	int points;
	int newEnemyTime = 0;
	Player* player;
	Background* background;
	bool controlContinue = false;
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	bool controlDash = false;
	bool isDashButtonPressed = false;
	
	list<Actor*> balas;
	bool controlChange = false;
	void kill();

	int time = 0;
	int frame = 1;
	Text* chrono;
	void formatTime();
	list<Projectile*> enemyProjectiles;
};

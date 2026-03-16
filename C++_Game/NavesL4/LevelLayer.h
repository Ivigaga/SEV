#pragma once
#include "Layer.h"
#include "Actor.h"
#include "Text.h"
#include "Background.h"

class LevelLayer : public Layer
{
public:
	LevelLayer(Game* game);
	void init() override;
	void draw() override;
	void update() override;
	void processControls() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event);
	void gamePadToControls(SDL_Event event);
	bool controlContinue;
	SDL_GameController* gamePad; // Mando
	Background* background;
	Actor* button1;
	Actor* button2;
	Actor* button3;
	int pos = 1;
	int maxLevel = 3;
	int minLevel = 1;
	Actor* player;
	int delay = 0;
	Text* chrono;
	void formatTime(int time);
};

#include "LevelLayer.h"

LevelLayer::LevelLayer(Game* game)
	: Layer(game) {
	init();
	gamePad = SDL_GameControllerOpen(0);
}

void LevelLayer::init() {
	// Fondo normal, sin velocidad
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, game);
	button1 = new Actor("res/boton1.png", WIDTH * 0.25, HEIGHT * 0.5, 50, 50, game);
	button2 = new Actor("res/boton2.png", WIDTH * 0.5, HEIGHT * 0.5, 50, 50, game);
	button3 = new Actor("res/boton3.png", WIDTH * 0.75, HEIGHT * 0.5, 50, 50, game);
	player = new Actor("res/player_menu.png", WIDTH * 0.25, HEIGHT * 0.5,23,27,game);
	chrono = new Text("00:00:000", WIDTH * 0.25, HEIGHT * 0.33,game);
}

void LevelLayer::draw() {
	background->draw();
	button1->draw();
	button2->draw();
	button3->draw();
	player->draw();
	if (game->progression[pos - 1]) {
		chrono->draw();
	}
	SDL_RenderPresent(game->renderer); // Renderiza NO PUEDE FALTAR
}

void LevelLayer::update() {
	delay--;
	if (delay < 0) {
		delay == 0 ;
	}
	if (pos == 1) {
		player->x = button1->x;
		chrono->x = button1->x;
	}
	else if (pos == 2) {
		player->x = button2->x;
		chrono->x = button2->x;
	}
	else if (pos == 3) {
		player->x = button3->x;
		chrono->x = button3->x;
	}
	formatTime(game->times[pos-1]);


	if (controlContinue) {
		// Cambia la capa
		game->currentLevel = pos - 1;
		game->layer = game->gameLayer;
		game->gameLayer->init();
		controlContinue = false;
	}
}

void LevelLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			gamePad = SDL_GameControllerOpen(0);
			if (gamePad == NULL) {
				cout << "error en GamePad" << endl;
			}
			else {
				cout << "GamePad conectado" << endl;
			}
		}
		// Cambio automático de input
		// PONER el GamePad
		if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERAXISMOTION) {
			game->input = game->inputGamePad;
		}
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}
		// Procesar teclas
		if (game->input == game->inputGamePad) {  // gamePAD
			gamePadToControls(event);
		}
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
	}
	
}

void LevelLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: // derecha
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_SPACE: // dispara
			controlContinue = true;
			break;
		case SDLK_a:
			if (pos > minLevel)
				pos--;
			break;
		case SDLK_d:
			if (pos <maxLevel)
				pos++;
			break;
		}
		
	}
}

void LevelLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;

	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (button1->containsPoint(motionX, motionY)) {
			pos = 1;
			controlContinue = true;
		}
		else if (button2->containsPoint(motionX, motionY)) {
			pos = 2;
			controlContinue = true;
		}
		else if (button3->containsPoint(motionX, motionY)) {
			pos = 3;
			controlContinue = true;
		}
	}
}

void LevelLayer::gamePadToControls(SDL_Event event) {
	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A);
	int stickX = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTX);
	if (buttonA) {
		controlContinue = true;
	}
	if (delay<=0)
	{
		if (stickX > 4000) {
			if (pos < maxLevel)
				pos++;
		}
		else if (stickX < -4000) {
			if (pos > minLevel)
				pos--;

		}
		delay = 4;
	}
}


void LevelLayer::formatTime(int time) {
	int mil = time % 1000;
	int seg = time / 1000;
	int min = seg / 60;
	seg = seg % 60;
	string segundos = to_string(seg);
	string minutos = to_string(min);
	string milesimas = to_string(mil);
	if (mil == 1000) {
		mil = 0;
	}
	if (seg < 10) {
		segundos = "0" + to_string(seg);
	}
	if (min < 10) {
		minutos = "0" + to_string(min);
	}
	if (mil < 10) {
		milesimas = "00" + to_string(mil);
	}
	else if (mil < 100) {
		milesimas = "0" + to_string(mil);
	}

	chrono->content = minutos + ":" + segundos + ":" + milesimas;
}
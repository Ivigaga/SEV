#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	pause = false;
	//message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
	//	WIDTH, HEIGHT, game);
	gamePad = SDL_GameControllerOpen(0);
	//init();
}

void GameLayer::init() {
	controlChange = false;
	controlDash = false;
	controlMoveX = false;
	controlMoveY = false;
	controlShoot = false;
	frame = 1;
	time = 0;
	chrono = new Text("00:00:000",61,13,game);
	//pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);
	//buttonJump = new Actor("res/boton_salto.png", WIDTH * 0.9, HEIGHT * 0.55, 100, 100, game);
	//buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);
	space = new Space(0.8);
	scrollX = 0;
	//Ext-Scroll
	scrollY = 0;
	tiles.clear();
	audioBackground = new Audio("res/music.wav", true);
	audioBackground->play();

	points = 0;

	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	enemyProjectiles.clear();
	balas.clear();

	//loadMap("res/0.txt");
	loadMap("res/" + to_string(game->currentLevel) + ".txt");
}

void GameLayer::processControls() {
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
		if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION) {
			game->input = game->inputMouse;
		}
		// Procesar Mando
		if (game->input == game->inputGamePad) {  // gamePAD
			gamePadToControls(event);
		}
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
			mouseToControls(event);
		}
		if (game->input == game->inputMouse) {
			keysToControls(event);
			mouseToControls(event);
		}
	}
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}
	//procesar controles
	//procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		controlShoot = false;
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
			int i = 0;
			for (auto const& bullet : balas)
			{
				i++;
				if (i <= player->weapon->bullets) {
					bullet->texture = game->getTexture("res/icono_balas.png");
				}
				else {
					bullet->texture = game->getTexture("res/icono_balas_recarga.png");
				}
			}
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	//else {
	//	player->moveX(0);
	//}

	/*
	if (controlChangeX > 0) {
		player->orientation = game->orientationRight;
	}
	else if (controlChangeX < 0) {
		player->orientation = game->orientationLeft;
	}
	*/

	// Eje Y
	if (controlMoveY > 0) {
		
	}
	else if (controlMoveY < 0) {
		player->jump();
	}
	else {

	}

	if (controlDash) {
		player->dash();
		if (isDashButtonPressed) {
			controlDash = false;
		}
	}

	if (controlChange) {
		player->changeWeapon();
		balas.clear();
		for (int i = 0; i < player->weapon->amo; i++)
		{
			string file;
			if (i+1 <= player->weapon->bullets) {
				file = "res/icono_balas.png";

			}
			else {
				file="res/icono_balas_recarga.png";
			}
			if (player->weapon->amo % 2 == 0) {
				balas.push_back(new Actor(file, player->x - player->width / 2 + 16 * (player->weapon->amo - 2) / 2 - 16 * i, player->y - player->height / 2 - 16, 16, 16, game));
			}
			else {
				balas.push_back(new Actor(file, player->x + (16 * (player->weapon->amo - 1) / 2) - 16 * i, player->y - player->height / 2 - 16, 16, 16, game));
			}

			
		}
	}

}

void GameLayer::update() {
	if (pause) {
		controlChange = false;
		controlMoveX = false;
		controlDash = false;
		controlMoveY = false;
		controlShoot = false;
		isDashButtonPressed = false;
		return;
	}
	else {
		time += 33;
		if (frame == 3) {
			time += 1;
			frame = 0;
		}
		if (time > 3000) {
			int a = 0;
		}
		frame++;
		formatTime();
	}
	// Nivel superado
	if (cup->isOverlap(player)) {
		controlContinue == false;
		game->layer = game->levelLayer;
		int bestTime = game->times[game->currentLevel];
		if (game->progression[game->currentLevel] == false) {
			game->times[game->currentLevel] = time;
		}
		else {
			if (bestTime > time) {
				game->times[game->currentLevel] = time;
			}
		}
		game->progression[game->currentLevel] = true;
		Mix_CloseAudio();
	}

	// Jugador se cae
	if (player->y >= mapHeight) {
		kill();
	}

	space->update();
	background->setSpeed(player->vx/7);
	background->update();
	player->update();
	for (auto const& enemy : enemies) {
		
		if (enemy->isInRender(scrollX, scrollY) && enemy->seesPlayer(player)) {
			Projectile* p = enemy->attack();
			if (p != NULL) {
				space->addDynamicActor(p);
				enemyProjectiles.push_back(p);
			}

		}
		enemy->update();
	}
	for (auto const& projectile : projectiles) {
		projectile->update();
	}
	for (auto const& projectile : enemyProjectiles) {
		projectile->update();
	}

	
	

	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy) && !player->dashing && enemy->state!=game->stateDying && enemy->state != game->stateDead) {
			player->loseLife();
			if (player->lifes <= 0) {
				kill();
				return;
			}
		}
	}
	// Colisiones , Enemy - Projectile

	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<Projectile*> deleteEnemyProjectiles;
	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX, scrollY) == false || projectile->shouldDie() ) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
		if (!projectile->timeDependant)
		{
			for (auto const& tile : tiles) {
				if (projectile->isOverlap(tile)) {

					bool pInList = std::find(deleteProjectiles.begin(),
						deleteProjectiles.end(),
						projectile) != deleteProjectiles.end();

					if (!pInList) {
						deleteProjectiles.push_back(projectile);
					}
				}

			}
		}
	}
	for (auto const& projectile : enemyProjectiles) {
		if (projectile->isInRender(scrollX, scrollY) == false || projectile->shouldDie()) {

			bool pInList = std::find(deleteEnemyProjectiles.begin(),
				deleteEnemyProjectiles.end(),
				projectile) != deleteEnemyProjectiles.end();

			if (!pInList) {
				deleteEnemyProjectiles.push_back(projectile);
			}
		}
		else {
			if (projectile->isOverlap(player)) {
				bool pInList = std::find(deleteEnemyProjectiles.begin(),
					deleteEnemyProjectiles.end(),
					projectile) != deleteEnemyProjectiles.end();

				if (!pInList) {
					deleteEnemyProjectiles.push_back(projectile);
				}

				player->loseLife();
				if (player->lifes <= 0) {
					kill();
					return;
				}
			}
		}
	}

	for (auto const& enemy : enemies) {
		if (enemy->state != game->stateDying && enemy->state != game->stateDead)
		{
			for (auto const& projectile : projectiles) {
				if (enemy->isOverlap(projectile)) {
					bool pInList = std::find(deleteProjectiles.begin(),
						deleteProjectiles.end(),
						projectile) != deleteProjectiles.end();

					if (!pInList) {
						deleteProjectiles.push_back(projectile);
					}


					enemy->impacted(projectile->damage);
					if (enemy->state == game->stateDying) {
						player->weapon->rechargeTime = player->weapon->timeForBullet;
						player->weapon->recharge();
					}


				}
			}
		}
	}
	int i = 0;
	for (auto const& bullet : balas)
	{
		if (player->weapon->amo % 2 == 0) {


			bullet->x = player->x + player->width / 4 + 16 * (player->weapon->amo - 2) / 2 - 16 * i;
			bullet->y = player->y - player->height / 2 - 16;
			
		}
		else {
			
			bullet->x = player->x + (16 * (player->weapon->amo - 1) / 2) - 16 * i;
			bullet->y = player->y - player->height / 2 - 16;
			
		}
		i++;
		if (i <= player->weapon->bullets) {
			if(bullet->texture != game->getTexture("res/icono_balas.png") ){
				bullet->texture = game->getTexture("res/icono_balas.png");
			}
			
		}
	}

	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();
			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	for (auto const& delProjectile : deleteEnemyProjectiles) {
		enemyProjectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteEnemyProjectiles.clear();



	cout << "update GameLayer" << endl;
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		int nLines = 0;
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}
			nLines++;
			cout << character << endl;
		}
		mapHeight = nLines * 32;
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case 'C': {
		loadMapObject('.', x, y);
		cup = new Tile("res/copa.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		cup->y = cup->y - cup->height / 2;
		space->addDynamicActor(cup); // Realmente no hace falta
		break;
	}
	case 'E': {
		// Primero añadir Tile - Fondo en la misma posición
		loadMapObject('.', x, y);
		Enemy* enemy = new SmallEnemy(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemy->startX = x;
		enemy->startY = enemy->y;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case 'B': {
		// Primero añadir Tile - Fondo en la misma posición
		loadMapObject('.', x, y);
		Enemy* enemy = new BigEnemy(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemy->startX = x;
		enemy->startY = enemy->y;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		break;
	}
	case '1': {
		// Primero añadir Tile - Fondo en la misma posición
		loadMapObject('.', x, y);
		player = new Player(x, y, game);
		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		player->startX = x;
		player->startY = player->y;
		space->addDynamicActor(player);
		for (int i = 0; i < player->weapon->bullets; i++)
		{
			if (player->weapon->bullets % 2 == 0) {

				balas.push_back(new Actor("res/icono_balas.png", player->x - player->width / 4 + 16*(player->weapon->bullets -2)/2 -16*i, player->y - player->height / 2 - 16, 16, 16, game));
				

			}
			else {
				
				balas.push_back(new Actor("res/icono_balas.png", player->x + (16 * (player->weapon->bullets - 1) / 2)-16*i, player->y - player->height / 2 - 16, 16, 16, game));
				
			}
		}
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/suelo.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'R': {
		Tile* tile = new Tile("res/relleno.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'T': {
		Tile* tile = new Tile("res/techo.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'D': {
		Tile* tile = new Tile("res/pared_borde_derecha.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'I': {
		Tile* tile = new Tile("res/pared_borde_izquierda.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '2': {
		Tile* tile = new Tile("res/esquina_abajo_derecha.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '3': {
		Tile* tile = new Tile("res/esquina_abajo_izquierda.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '4': {
		Tile* tile = new Tile("res/esquina_arriba_derecha.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '5': {
		Tile* tile = new Tile("res/esquina_arriba_izquierda.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '6': {
		Tile* tile = new Tile("res/esquina_abajo_derecha_i.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '7': {
		Tile* tile = new Tile("res/esquina_abajo_izquierda_i.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '8': {
		Tile* tile = new Tile("res/esquina_arriba_derecha_i.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '9': {
		Tile* tile = new Tile("res/esquina_arriba_izquierda_i.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'i': {
		Tile* tile = new Tile("res/pared_izquierda.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'd': {
		Tile* tile = new Tile("res/pared_derecha.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	}
}


void GameLayer::calculateScroll() {
	// limite izquierda
	if (player->x > WIDTH * 0.3) {
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.3) {
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}

	// limite arriba
	if (player->y > mapHeight *0.25) {
		if (player->y + scrollY < HEIGHT * 0.25) {
			scrollY = -(player->y - HEIGHT * 0.25);
		}
	}

	// limite abajo
	if (player->y < mapHeight - HEIGHT*0.25) {
		if (player->y + scrollY > HEIGHT * 0.75) {
			scrollY = -(player->y - HEIGHT *0.75);
		}
	}
}


void GameLayer::draw() {
	calculateScroll();

	background->draw();

	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}
	for (auto const& bullet : balas) {
		bullet->draw(scrollX, scrollY);
	}
	for (auto const& projectile : enemyProjectiles) {
		projectile->draw(scrollX, scrollY);
	}
	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}
	cup->draw(scrollX, scrollY);
	
	player->draw(scrollX, scrollY);
	if (player->aimingAt == player->ARRIBA || (player->aimingAt == player->NEUTRAL && player->direccion == player->ARRIBA)) {
		if(!player->dashing)
			player->weapon->draw(scrollX, scrollY);
	}

	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}
	chrono->draw();
	// HUD
	if (game->input == game->inputMouse) {
		//buttonJump->draw(); // NO TIENEN SCROLL, POSICION FIJA
		//buttonShoot->draw(); // NO TIENEN SCROLL, POSICION FIJA
		//pad->draw(); // NO TIENEN SCROLL, POSICION FIJA
	}
	if (pause) {
		//message->draw();
		
	}
	SDL_RenderPresent(game->renderer); // Renderiza
}


void GameLayer::keysToControls(SDL_Event event) {
	if (!pause)
	{
		if (event.type == SDL_KEYDOWN) {
			int code = event.key.keysym.sym;
			// Pulsada
			switch (code) {
			case SDLK_ESCAPE:
				game->loopActive = false;
				break;
			case SDLK_1:
				game->scale();
				break;
			case SDLK_d: // derecha
				controlMoveX = 1;
				player->direccion = player->DERECHA;
				break;
			case SDLK_a: // izquierda
				controlMoveX = -1;
				player->direccion = player->IZQUIERDA;
				break;
			case SDLK_w: // arriba
				controlMoveY = -1;
				if(player->onAir)
					player->direccion = player->ARRIBA;
				break;
			case SDLK_s: // abajo
				player->direccion = player->ABAJO;
				controlMoveY = 1;
				break;
			case SDLK_SPACE: // dispara
				controlShoot = true;
				break;
			case SDLK_UP:
				player->aimingAt = player->ARRIBA;
				break;
			case SDLK_DOWN:
				player->aimingAt = player->ABAJO;
				break;
			case SDLK_RIGHT:
				player->aimingAt = player->DERECHA;
				break;
			case SDLK_LEFT:
				player->aimingAt = player->IZQUIERDA;
				break;
			case SDLK_e:
				controlChange = true;
				break;
			case SDLK_c:
				if (!isDashButtonPressed)
				{
					controlDash = true;
					isDashButtonPressed = true;
				}
				break;
			}
		}
		if (event.type == SDL_KEYUP) {
			int code = event.key.keysym.sym;
			// Levantada
			switch (code) {
			case SDLK_d: // derecha
				if (controlMoveX == 1) {
					player->direccion = player->NEUTRAL;
					controlMoveX = 0;
				}
				break;
			case SDLK_a: // izquierda
				if (controlMoveX == -1) {
					player->direccion = player->NEUTRAL;
					controlMoveX = 0;
				}
				break;
			case SDLK_w: // arriba
				if (controlMoveY == -1) {
					player->direccion = player->NEUTRAL;
					controlMoveY = 0;
				}
				break;
			case SDLK_s: // abajo
				if (controlMoveY == 1) {
					player->direccion = player->NEUTRAL;
					controlMoveY = 0;
				}
				break;
			case SDLK_SPACE: // dispara
				controlShoot = false;
				break;
			case SDLK_UP:
				player->aimingAt = player->NEUTRAL;
				break;
			case SDLK_DOWN:
				player->aimingAt = player->NEUTRAL;
				break;
			case SDLK_RIGHT:
				player->aimingAt = player->NEUTRAL;
				break;
			case SDLK_LEFT:
				player->aimingAt = player->NEUTRAL;
				break;
			case SDLK_e:
				controlChange = false;
				break;
			case SDLK_c:
				controlDash = false;
				isDashButtonPressed = false;
				break;
			}

		}
		if (event.type == SDL_QUIT) {
			game->loopActive = false;
		}
	}
}

void GameLayer::mouseToControls(SDL_Event event) {
    // Modificación de coordenadas por posible escalado
    float motionX = event.motion.x / game->scaleLower;
    float motionY = event.motion.y / game->scaleLower;

    // Cada vez que hacen click
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            controlContinue = true;
            if (!pause) {
                controlShoot = true; // Acción para el botón izquierdo
            }
        } else if (event.button.button == SDL_BUTTON_RIGHT) {
			controlChange = true;
        }
    }

    // Cada vez que se mueve
    if (event.type == SDL_MOUSEMOTION && !pause) {
        if (abs(motionX - (WIDTH / 2)) / WIDTH > abs(motionY - (HEIGHT / 2)) / HEIGHT) {
            if (motionX > WIDTH / 2) {
                player->aimingAt = player->DERECHA;
            } else {
                player->aimingAt = player->IZQUIERDA;
            }
        } else if (abs(motionX - (WIDTH / 2)) / WIDTH < abs(motionY - (HEIGHT / 2)) / HEIGHT) {
            if (motionY > HEIGHT / 2) {
                player->aimingAt = player->ABAJO;
            } else {
                player->aimingAt = player->ARRIBA;
            }
        }
    }

    // Cada vez que levantan el click
    if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            controlShoot = false; // Acción al soltar el botón izquierdo
        } else if (event.button.button == SDL_BUTTON_RIGHT) {
			controlChange = false;
        }
    }
}

void GameLayer::gamePadToControls(SDL_Event event) {

	
		// Leer los botones
		bool buttonA = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A);
		bool buttonX = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_X);
		bool buttonB = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_B);
		// SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B
		// SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y
		cout << "botones:" << buttonA << "," << buttonB << endl;
		int stickX = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTX);
		int stickY = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTY);
		int stickXDer = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_RIGHTX);
		int stickYDer = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_RIGHTY);
		int triggerR = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		int triggerL = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
		cout << "stickX" << stickX << endl;
		cout << "derecha" << stickXDer << endl;
		// Retorna aproximadamente entre [-32800, 32800], el centro debería estar en 0
		// Si el mando tiene "holgura" el centro varia [-4000 , 4000]
		if (pause) {
			if (buttonA || buttonB || buttonX)
				controlContinue = true;
		}
		if (!pause)
		{
		if (stickX > 4000) {
			player->direccion = player->DERECHA;
			controlMoveX = 1;
		}
		else if (stickX < -4000) {
			player->direccion = player->IZQUIERDA;
			controlMoveX = -1;
		}
		else {
			controlMoveX = 0;
			player->direccion = player->NEUTRAL;
		}

		if (abs(stickY) > abs(stickX) && abs(stickY) > 4000) {
			if (stickY > 0) {
				player->direccion = player->ABAJO;
			}if (stickY < 0) {
				player->direccion = player->ARRIBA;
			}
		}



		if (buttonB || triggerR > 4000) {
			controlShoot = true;
		}
		else {
			controlShoot = false;
		}

		if (triggerL > 4000) {
			controlChange = true;
		}
		else {
			controlChange = false;
		}


		if (abs(stickYDer) >= abs(stickXDer)) {
			if (stickYDer > 4000) {
				player->aimingAt = player->ABAJO;
			}
			else if (stickYDer < -4000) {
				player->aimingAt = player->ARRIBA;
			}
			else {
				player->aimingAt = player->NEUTRAL;
			}
		}
		else {
			if (stickXDer > 4000) {
				player->aimingAt = player->DERECHA;
			}
			else if (stickXDer < -4000) {
				player->aimingAt = player->IZQUIERDA;
			}
			else {
				player->aimingAt = player->NEUTRAL;
			}
		}

		if (buttonA) {
			controlMoveY = -1; // Saltar
		}
		else {
			controlMoveY = 0;
		}

		if (event.type == SDL_CONTROLLERBUTTONDOWN) {
			if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X) {
				if (!isDashButtonPressed) {
					controlDash = true;
					isDashButtonPressed = true;
				}
			}
		}
		else if (event.type == SDL_CONTROLLERBUTTONUP) {
			if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X) {
				isDashButtonPressed = false;
				controlDash = false;
			}
		}
	}
	
}

void GameLayer::kill() {
	player->restart(); 
	list<Enemy*> deleteEnemies;
	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead || enemy->state == game->stateDying) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();
			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}
	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();
	for (auto const& enemy : enemies) {
		enemy->restart();
	}
	for (auto const& projectile : enemyProjectiles) {
		space->removeDynamicActor(projectile);
		delete projectile;
	}
	enemyProjectiles.clear();
	for (auto const& projectile : projectiles) {
		space->removeDynamicActor(projectile);
		delete projectile;
	}
	controlChange = false;
	controlDash = false;
	controlMoveX = false;
	controlMoveY = false;
	controlShoot = false;
	projectiles.clear();
	scrollX = 0;
}

void GameLayer::formatTime() {
	int mil=time%1000;
	int seg=time/1000;
	int min=seg/60;
	seg = seg % 60;
	string segundos= to_string(seg);
	string minutos= to_string(min);
	string milesimas= to_string(mil);
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
	
	chrono->content;
	chrono->content = minutos + ":" + segundos + ":" + milesimas;
}
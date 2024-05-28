#include "Game.h"
#include "GameObject.h"
#include "Sheep.h"
#include "Sword.h"
#include <string.h>

Sheep* sheep;
GameObject* titleScreen;
Sword* sword;

Game::Game(const char* title, bool fullscreen) {
	room = "Menu";
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystem initialized..." << std::endl;
		//Get current display size
		SDL_DisplayMode DM;
		if (SDL_GetDesktopDisplayMode(0, &DM) != 0) {
			std::cout << SDL_GetError() << std::endl;
		}
		width = DM.w;
		height = DM.h;
		//Create window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		if (window == NULL) {
			std::cout << SDL_GetError() << std::endl;
		}
		//SDL_SetWindowResizable(window, SDL_TRUE);
		//Create renderer and game objects
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			sheep = new Sheep("assets/sheep1.png", renderer, 100, 0.1, 0.1);

			titleScreen = new GameObject("assets/title.png", renderer, 250);
			titleScreen->x = width / 2;
			titleScreen->y = height / 2;

			sword = new Sword("assets/sword.png", renderer, 50, 0.3, 500, sheep);
		}
		else {
			std::cout << SDL_GetError() << std::endl;
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}
}

Game::~Game() {}

void Game::handleEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			//Only flip is Space is JUST NOW being pressed down
			if (!keys.contains("Space") && strcmp(SDL_GetKeyName(event.key.keysym.sym), "Space") == 0) {
				if (room != "Menu") {
					sheep->flip = 1 - sheep->flip;
				}
			}
			keys.insert(std::string(SDL_GetKeyName(event.key.keysym.sym))); //add keydown to keys set
			break;
		case SDL_KEYUP:
			keys.erase(std::string(SDL_GetKeyName(event.key.keysym.sym))); //remove keyup from keys set
			break;
		}
	}
}

void Game::update(int frame) {
	if (room != "Menu") {
		//Sheep update
		if (keys.contains("W")) {
			sheep->move(frame);
		}
		if (keys.contains("A")) {
			sheep->rotate(frame);
		}
		if (keys.contains("S")) {
			sheep->move(-frame);
		}
		if (keys.contains("D")) {
			sheep->rotate(-frame);
		}

		//Sword update
		sword->sheepify();
		if (!sword->out()) {
			if (keys.contains("O")) {
				sword->beginPoke();
			} else if (keys.contains("P")) {
				sword->beginSwing();
			}
		}
		sword->swing(frame);
	}
	if (room == "Menu") {
		if (keys.contains("Return")) {
			room = "Level1";
		}
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	if (room != "Menu") {
		sheep->render();
		sword->renderSwing();
	}
	if (room == "Menu") {
		titleScreen->render();
	}
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned..." << std::endl;
}

bool Game::running() { 
	return isRunning; 
}

SDL_Renderer* Game::getRenderer() {
	return renderer;
}

int Game::getWidth() {
	return width;
}

int Game::getHeight() {
	return height;
}



#include "Game.h"
#include "GameObject.h"
#include "Sheep.h"
#include "Sword.h"
#include <string.h>

Sheep* sheep;
Sword* sword;

SDL_Color black = { 0, 0, 0 };
SDL_Color orange = { 255, 165, 0 };
SDL_Color green = { 0, 255, 0 };
SDL_Color blue = { 0, 0, 255 };

GameObject* progressText;
GameObject* titleText;

Game::Game(const char* title, bool fullscreen) {
	room = "Menu";
	level = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0) {
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

			sword = new Sword("assets/sword.png", renderer, 50, 0.3, 250, sheep);

			progressText = new GameObject("Press Enter to progress", "BlackRunters", 250, black, orange, 1000, renderer, 250);
			progressText->x = width - progressText->width / 2;
			progressText->y = height - progressText->height / 2;

			titleText = new GameObject("Life of Sheep", "BlackRunters", 250, blue, green, 3000, renderer, 150);
			titleText->x = width / 2;
			titleText->y = height / 2;
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
	for (std::string i : keys) {
		currentKeys.erase(i);
	}
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (!keys.contains(std::string(SDL_GetKeyName(event.key.keysym.sym)))) {
				currentKeys.insert(std::string(SDL_GetKeyName(event.key.keysym.sym)));
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
	if (room != "Menu" && room != "Prepare") {
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
		if (currentKeys.contains("Space")) {
			sheep->flip = 1 - sheep->flip;
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
		if (currentKeys.contains("Return")) {
			prepare();
		}
	} else if (room == "Prepare") {
		if (currentKeys.contains("Return")) {
			levelup();
		}
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	if (room != "Menu") {
		sheep->render();
		if (room != "Prepare") {
			sword->renderSwing();
		}
	}
	if (room == "Menu") {
		progressText->render();
		titleText->render();
	} else if (room == "Prepare") {
		progressText->render();
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

void Game::prepare() {
	room = "Prepare";
	sheep->x = 2 * sheep->width + 100;
	sheep->y = 2 * sheep->height + 100;
	sheep->width *= 4;
	sheep->height *= 4;
	sheep->flip = 0;
	sheep->angle = 0;
}

void Game::levelup() {
	level++;
	room = "level" + level;
	sheep->width /= 4;
	sheep->height /= 4;
	sheep->x = sheep->width / 2;
	sheep->y = sheep->height / 2;
}



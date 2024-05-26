#include "Game.h"
#include "GameObject.h"

GameObject* sheep;

Game::Game(const char* title, bool fullscreen) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		SDL_DisplayMode DM;
		if (SDL_GetDesktopDisplayMode(0, &DM) != 0) {
			std::cout << SDL_GetError() << std::endl;
		}
		width = DM.w;
		height = DM.h;
		std::cout << "Subsystem initialized..." << std::endl;
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		if (window == NULL) {
			std::cout << SDL_GetError() << std::endl;
		}
		//SDL_SetWindowResizable(window, SDL_TRUE);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			sheep = new GameObject("assets/sheep1.png", renderer, 255);
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
			keys.insert(std::string(SDL_GetKeyName(event.key.keysym.sym)));
			break;
		case SDL_KEYUP:
			keys.erase(std::string(SDL_GetKeyName(event.key.keysym.sym)));
			break;
		case SDL_WINDOWEVENT_RESIZED:
			std::cout << "display event" << std::endl;
		default:
			break;
		}
	}
	/**
	for (std::string i : keys) {
		std::cout << i << ' ';
	}
	std::cout <<keys.size() << '\n';
	**/
}

void Game::update(int frame) {
	sheep->angle += frame / 10;
}

void Game::render() {
	SDL_RenderClear(renderer);
	sheep->render();
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



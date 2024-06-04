#include "Game.h"
#include "GameObject.h"
#include "Sheep.h"
#include "Sword.h"
#include "Bull.h"
#include "TextArea.h"
#include <SDL_mixer.h>
#include <string>

Sheep* sheep;
const int sheepBaseHealth = 300;

Sword* sword;
const int swordBaseSwingDamage = 30;
const int swordBasePokeTime = 260;
bool swordOut = false;

SDL_Color red = { 255, 0, 0 };
SDL_Color black = { 0, 0, 0 };
SDL_Color orange = { 255, 165, 0 };
SDL_Color green = { 0, 255, 0 };
SDL_Color blue = { 0, 0, 255 };

TextArea* progressText;
TextArea* titleText;
TextArea* upgradeText;
TextArea* healthText;

Bull* bull;

Mix_Music* menuMusic;

Game::Game(const char* title, bool fullscreen) {
	room = "Menu";
	level = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0 && Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0) {
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
		menuMusic = Mix_LoadMUS("assets/a sheepish menu.ogg");
		Mix_PlayMusic(menuMusic, -1);
		//SDL_SetWindowResizable(window, SDL_TRUE);
		//Create renderer and game objects
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			progressText = new TextArea("Press Enter to progress", "BlackRunters", 250, black, orange, 1000, renderer, 250);
			progressText->x = width - progressText->width / 2;
			progressText->y = height - progressText->height / 2;

			titleText = new TextArea("Life of Sheep", "BlackRunters", 250, blue, green, 3000, renderer, 150);
			titleText->x = width / 2;
			titleText->y = height / 2;

			upgradeText = new TextArea("Press N to upgrade poke speed, M to upgrade swing damage. Progress upon upgrading", "BlackRunters", 250, black, orange, 2000, renderer, 250);
			upgradeText->y = height - upgradeText->height / 2;

			healthText = new TextArea(std::to_string(sheepBaseHealth), "BlackRunters", 250, red, black, 1000, renderer, 100);

			sheep = new Sheep("assets/sheep1.png", renderer, 100, 0.2, 0.1, sheepBaseHealth, healthText);

			sword = new Sword("assets/sword.png", renderer, 100, 0.3, swordBasePokeTime, sheep, 15, swordBaseSwingDamage);

			bull = new Bull("assets/bull.png", renderer, 100, 500, sword, sheep);
			bull->x = width - bull->width / 2;
			bull->y = height - bull->height / 2;

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
			sheep->angle = 90;
			sheep->flip = 0;
		}
		if (keys.contains("A")) {
			sheep->angle = 0;
			sheep->flip = 1;
		}
		if (keys.contains("S")) {
			sheep->angle = 270;
			sheep->flip = 0;
		}
		if (keys.contains("D")) {
			sheep->angle = 0;
			sheep->flip = 0;
		}
		if (keys.contains("W") || keys.contains("A") || keys.contains("S") || keys.contains("D")) {
			sheep->move(frame);
		}
		sheep->update(frame);

		//Sword update
		sword->sheepify();
		if (!sword->out()) {
			if (keys.contains("O")) {
				sword->beginPoke();
			} else if (keys.contains("P")) {
				sword->beginSwing();
			}
		}
		swordOut = sword->swing(frame);
	}
	if (room == "Menu") {
		if (currentKeys.contains("Return")) {
			prepare();
		}
	} else if (room == "Prepare") {
		if (currentKeys.contains("Return")) {
			levelup();
		}
		if (currentKeys.contains("N")) {
			sword->updatePokeTime(-swordBasePokeTime / 13);
			levelup();
		}
		else if (currentKeys.contains("M")) {
			sword->updateSwingDmg(swordBaseSwingDamage / 6);
			levelup();
		}
	}
	if (room == "Level1") {
		if (!bull->damaged(swordOut)) {
			bull->update(frame);
		}
		else {
			if (bull->defeated(frame)) {
				prepare();
			}
		}
	}
}


void Game::render() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	if (room != "Menu") {
		std::vector<double> corners(8);
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
		upgradeText->render();
	} else if (room == "Level1") {
		bull->render();
	}
	if (room.starts_with("Level")) {
		healthText->render();
	}
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_FreeMusic(menuMusic);
	Mix_Quit();
	IMG_Quit();
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
	Mix_HaltMusic();
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
	room = "Level";
	room = room + std::to_string(level);
	sheep->width /= 4;
	sheep->height /= 4;
	sheep->x = sheep->width / 2;
	sheep->y = sheep->height / 2;
}



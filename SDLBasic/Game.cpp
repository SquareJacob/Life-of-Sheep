#include "Game.h"
#include "GameObject.h"
#include "Sheep.h"
#include "Sword.h"
#include "Bull.h"
#include "TextArea.h"
#include "Bar.h"
#include "Cow.h"
#include "Dog.h"
#include "Chicken.h"
#include "Horse.h"
#include <SDL_mixer.h>

Sheep* sheep;
const int sheepBaseHealth = 300;
int gold = 0;

Sword* sword;
const int swordBaseSwingDamage = 45;
const int swordBasePokeTime = 260;

SDL_Color red = { 255, 0, 0 };
SDL_Color black = { 0, 0, 0 };
SDL_Color orange = { 255, 165, 0 };
SDL_Color green = { 0, 255, 0 };
SDL_Color blue = { 0, 0, 255 };
SDL_Color yellow = { 255, 255, 0 };

TextArea* progressText;
TextArea* titleText;
TextArea* upgradeText;
TextArea* goldText;

Bull* bull;
Dog* dog;
Chicken* chicken;
Horse* horse;
Cow* cow;

Mix_Music* menuMusic;
Mix_Music* bullMusic;
Mix_Music* mooMusic;
Mix_Music* dogMusic;
Mix_Music* chickenMusic;
Mix_Music* horseMusic;

Bar* sheepBar;
Bar* enemyBar;

GameObject* grass;

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
		battleX = (1 - edge) / 2.0 * width;
		battleY = (1 - edge) / 2.0 * height;
		//Create window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		if (window == NULL) {
			std::cout << SDL_GetError() << std::endl;
		}
		menuMusic = Mix_LoadMUS("assets/a sheepish menu.ogg");
		Mix_PlayMusic(menuMusic, -1);
		bullMusic = Mix_LoadMUS("assets/CHARGE!!.ogg");
		mooMusic = Mix_LoadMUS("assets/moo.ogg");
		dogMusic = Mix_LoadMUS("assets/lets play fetch.ogg");
		chickenMusic = Mix_LoadMUS("assets/cock.ogg");
		horseMusic = Mix_LoadMUS("assets/horsin' around.ogg");
		//SDL_SetWindowResizable(window, SDL_TRUE);
		//Create renderer and game objects
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			progressText = new TextArea("Press Enter to progress", "BlackRunters", 250, black, orange, 1000, renderer, height / 4);
			progressText->x = width - progressText->width / 2;
			progressText->y = height - progressText->height / 2;

			titleText = new TextArea("Life of Sheep", "BlackRunters", 250, blue, green, 3000, renderer, height / 6);
			titleText->x = width / 2;
			titleText->y = height / 2;

			upgradeText = new TextArea("Press N to upgrade poke speed, M to upgrade swing damage, each costing 1 gold", "BlackRunters", 250, black, orange, 2000, renderer, height / 4);
			upgradeText->y = height - upgradeText->height / 2;

			goldText = new TextArea("Gold :" + std::to_string(gold), "BlackRunters", 250, yellow, black, 10000, renderer, height / 10);
			updateGold(level);

			sheepBar = new Bar(sheepBaseHealth, sheepBaseHealth, green, red, 0, 0, height, true, renderer);
			sheepBar->updateHeight(battleX);

			enemyBar = new Bar(1, 1, red, blue, width - sheepBar->getHeight(), 0, height, true, renderer);
			enemyBar->updateHeight(battleX);

			sheep = new Sheep("assets/sheep1.png", renderer, height / 10, (float) height / 5000.0, 0.1, sheepBaseHealth, sheepBar);
			sheep->setBounds(edge);

			sword = new Sword("assets/sword.png", renderer, height / 10, 0.3, swordBasePokeTime, sheep, 15, swordBaseSwingDamage);

			grass = new GameObject("assets/grass.png", renderer, height * 2);
			grass->x = width / 2;
			grass->y = height / 2;

			bull = new Bull("assets/bull.png", renderer, height / 10, 3000, sword, sheep, enemyBar);
			bull->setBounds(edge);

			dog = new Dog("assets/dog.png", renderer, height / 15, 3000, sword, sheep, enemyBar);
			dog->setBounds(edge);

			chicken = new Chicken("assets/chicken.png", renderer, height / 15, 5000, sword, sheep, enemyBar, edge);
			chicken->setBounds(edge);

			horse = new Horse("assets/horse.png", renderer, height / 7, 6000, sword, sheep, enemyBar);
			horse->setBounds(edge);

			cow = new Cow("assets/cow.png", renderer, height / 10, 1000, sword, sheep, enemyBar);
			cow->x = width / 2;
			cow->y = cow->height * 1.5;

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
			if (keys.contains("A")) {
				sheep->angle = 315;
				sheep->flip = 1;
			}
			else if (keys.contains("D")) {
				sheep->angle = 45;
				sheep->flip = 0;
			}
			else {
				sheep->angle = 90;
				sheep->flip = 0;
			}
		}
		else if (keys.contains("S")) {
			if (keys.contains("A")) {
				sheep->angle = 45;
				sheep->flip = 1;
			}
			else if (keys.contains("D")) {
				sheep->angle = 315;
				sheep->flip = 0;
			}
			else {
				sheep->angle = 270;
				sheep->flip = 0;
			}
		}
		else if (keys.contains("A")) {
			sheep->angle = 0;
			sheep->flip = 1;
		}
		else if (keys.contains("D")) {
			sheep->angle = 0;
			sheep->flip = 0;
		}
		if (keys.contains("W") || keys.contains("A") || keys.contains("S") || keys.contains("D")) {
			sheep->move(frame);
		}
		sheep->update(frame);
		if (sheep->health == 0) {
			prepare();
			level -= 1;
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
		if (currentKeys.contains("N")) {
			if (updateGold(-1)) {
				sword->updatePokeTime(-swordBasePokeTime / 13);
			}
		}
		else if (currentKeys.contains("M")) {
			if (updateGold(-1)) {
				sword->updateSwingDmg(swordBaseSwingDamage / 6);
			}
		}
	}
	if (room == "Level1") {
		if (!bull->damaged()) {
			bull->update(frame);
		}
		else if (cow->ticker == -1) {
			if (cow->wiggle(frame)) {
				cow->ticker = 1;
				Mix_PlayMusic(mooMusic, -1);
			}
		}
		else if (cow->ticker == 1) {
			cow->speak("moo-hoo-hoo, what a suprise");		
			if (currentKeys.contains("Return")) {
				cow->ticker = 2;
			}
		}
		else if (cow->ticker == 2) {
			cow->speak("sheep beating bull, now that is new");
			if (currentKeys.contains("Return")) {
				cow->ticker = 3;
			}
		}
		else if (cow->ticker == 3) {
			cow->speak("but how long can you last?");
			if (currentKeys.contains("Return")) {
				cow->ticker = 4;
			}
		}
		else if (cow->ticker == 4) {
			if (cow->wiggle(-frame)) {
				prepare();
				updateGold(1);
			}
		}
		else if (bull->defeated(frame)) {
			cow->ticker = -1;
			Mix_FadeOutMusic(1000);
		}
	}
	else if (room == "Level2") {
		if (!dog->damaged()) {
			dog->update(frame);
		}
		else if (cow->ticker == -1) {
			if (cow->wiggle(frame)) {
				cow->ticker = 1;
				Mix_PlayMusic(mooMusic, -1);
			}
		}
		else if (cow->ticker == 1) {
			cow->speak("impressive, udderly impressive");
			if (currentKeys.contains("Return")) {
				cow->ticker = 2;
			}
		}
		else if (cow->ticker == 2) {
			cow->speak("sheep defeating dog, what a tragedy");
			if (currentKeys.contains("Return")) {
				cow->ticker = 3;
			}
		}
		else if (cow->ticker == 3) {
			cow->speak("but to what end?");
			if (currentKeys.contains("Return")) {
				cow->ticker = 4;
			}
		}
		else if (cow->ticker == 4) {
			if (cow->wiggle(-frame)) {
				prepare();
				updateGold(1);
			}
		}
		else if (dog->defeated(frame)) {
			cow->ticker = -1;
			Mix_FadeOutMusic(1000);
		}
	}
	else if (room == "Level3") {
		if (!chicken->damaged()) {
			chicken->update(frame);
		}
		else if (cow->ticker == -1) {
			if (cow->wiggle(frame)) {
				cow->ticker = 1;
				Mix_PlayMusic(mooMusic, -1);
			}
		}
		else if (cow->ticker == 1) {
			cow->speak("you have gone far enough");
			if (currentKeys.contains("Return")) {
				cow->ticker = 2;
			}
		}
		else if (cow->ticker == 2) {
			cow->speak("sheep crushing chicken, the world is changed");
			if (currentKeys.contains("Return")) {
				cow->ticker = 3;
			}
		}
		else if (cow->ticker == 3) {
			cow->speak("but do you really think you can escape?");
			if (currentKeys.contains("Return")) {
				cow->ticker = 4;
			}
		}
		else if (cow->ticker == 4) {
			if (cow->wiggle(-frame)) {
				prepare();
				updateGold(1);
			}
		}
		else if (chicken->defeated(frame)) {
			cow->ticker = -1;
			Mix_FadeOutMusic(1000);
		}
	}
	else if (room == "Level4") {
		if (!horse->damaged()) {
			horse->update(frame);
		}
		else if (cow->ticker == -1) {
			if (cow->wiggle(frame)) {
				cow->ticker = 1;
				Mix_PlayMusic(mooMusic, -1);
			}
		}
		else if (cow->ticker == 1) {
			cow->speak("...");
			if (currentKeys.contains("Return")) {
				cow->ticker = 2;
			}
		}
		else if (cow->ticker == 2) {
			cow->speak("sheep");
			if (currentKeys.contains("Return")) {
				cow->ticker = 3;
			}
		}
		else if (cow->ticker == 3) {
			cow->speak("it is time");
			if (currentKeys.contains("Return")) {
				cow->ticker = 4;
			}
		}
		else if (cow->ticker == 4) {
			if (cow->wiggle(-frame)) {
				prepare();
				updateGold(1);
			}
		}
		else if (horse->defeated(frame)) {
			cow->ticker = -1;
			Mix_FadeOutMusic(1000);
		}
		}
}


void Game::render() {
	grass->render();
	if (room != "Menu") {
		sheep->render();
		if (room != "Prepare") {
			sword->renderSwing();
			cow->render();
		}
	}
	if (room == "Menu") {
		progressText->render();
		titleText->render();
	} else if (room == "Prepare") {
		progressText->render();
		upgradeText->render();
		goldText->render();
	} else if (room == "Level1") {
		bull->render();
	}
	else if (room == "Level2") {
		dog->render();
	}
	else if (room == "Level3") {
		chicken->render();
	}
	else if (room == "Level4") {
		horse->render();
	}
	if (room.starts_with("Level")) {
		sheepBar->render();
		enemyBar->render();
		//render edge of battlefield
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawLine(renderer, battleX, battleY, battleX + edge * width, battleY);
		SDL_RenderDrawLine(renderer, battleX + edge * width, battleY + edge * height, battleX + edge * width, battleY);
		SDL_RenderDrawLine(renderer, battleX + edge * width, battleY + edge * height, battleX, battleY + edge * height);
		SDL_RenderDrawLine(renderer, battleX, battleY, battleX, battleY + edge * height);
	}

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	Mix_FreeMusic(menuMusic);
	TTF_Quit();
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
	Mix_FadeOutMusic(1000);
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
	cow->ticker = 0;
	sheep->prepare();
	Mix_HaltMusic();
	switch (level) {
	case 1:
		bull->prepare();
		Mix_PlayMusic(bullMusic, -1);
		break;
	case 2:
		dog->prepare();
		Mix_PlayMusic(dogMusic, -1);
		break;
	case 3:
		chicken->prepare();
		Mix_PlayMusic(chickenMusic, -1);
		break;
	case 4:
		horse->prepare();
		Mix_PlayMusic(horseMusic, -1);
	}
}

bool Game::updateGold(int amount) {
	gold += amount;
	if (gold >= 0) {
		goldText->changeText("Gold: " + std::to_string(gold));
		goldText->x = width - goldText->width / 2;
	}
	else {
		gold -= amount;
		return false;
	}
	return true;
}



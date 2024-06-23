/**
* TODO:
* -"Text has zero width"
* -Reset button
**/

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
#include "Button.h"
#include "Wolf.h"

Sheep* sheep;
const int sheepBaseHealth = 300;
int gold = 0;

Sword* sword;
int flyDistance;

TextArea* progressText;
TextArea* titleText;
TextArea* upgradeText;

TextArea* goldText;
int maxUpgrades = 3;
Button* upgradePoke;
int pokeUpgrades = 0;
Button* upgradeSwing;
int swingUpgrades = 0;
Button* upgradeFling;
int flingUpgrades = 0;

TextArea* farmerText;
double farmerShake;

Bull* bull;
Dog* dog;
Chicken* chicken;
Horse* horse;
Cow* cow;
Wolf* wolf1, *wolf2, *wolf3;

std::vector<Mix_Music*> music;
Mix_Music* menuMusic;
Mix_Music* bullMusic;
Mix_Music* mooMusic;
Mix_Music* dogMusic;
Mix_Music* chickenMusic;
Mix_Music* horseMusic;
Mix_Music* cow1Music;
Mix_Music* cow2Music;
Mix_Music* handMusic;
Mix_Music* wolfMusic;

Bar* sheepBar;
Bar* sheepBar2;
Bar* enemyBar;
Bar* enemyBar2;

GameObject* grass;
double gDis;

SDL_Rect* blackScreen = new SDL_Rect();
Button* leaveButton;

bool cutScene = false;
uint8_t cutSceneFrame = 0;

std::vector<Hand*> hands;

double radius;
int x = 0, y = 0;

std::vector<std::vector<int>> circles;

Game::Game(const char* title, bool fullscreen) {
	room = "Menu";
	level = 5;
	edge = originalEdge;
	mouseX = &x;
	mouseY = &y;
	buttons = new std::set<int>();
	currentButtons = new std::set<int>();
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0 && Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0) {
		std::cout << "Subsystem initialized..." << std::endl;
		//Get current display size
		SDL_DisplayMode DM;
		if (SDL_GetDesktopDisplayMode(0, &DM) != 0) {
			std::cout << SDL_GetError() << std::endl;
		}
		width = DM.w;
		height = DM.h;
		radius = width > height ? width : height;
		battleX = (1 - edge) / 2.0 * width;
		battleY = (1 - edge) / 2.0 * height;
		//Create window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		if (window == NULL) {
			std::cout << SDL_GetError() << std::endl;
		}
		menuMusic = createMusic("assets/a sheepish menu.ogg");
		bullMusic = createMusic("assets/CHARGE!!.ogg");
		mooMusic = createMusic("assets/moo.ogg");
		dogMusic = createMusic("assets/lets play fetch.ogg");
		chickenMusic = createMusic("assets/cock.ogg");
		horseMusic = createMusic("assets/horsin' around.ogg");
		cow1Music = createMusic("assets/a magic cow.ogg");
		cow2Music = createMusic("assets/YET TO BE FORGOTTEN.ogg");
		handMusic = createMusic("assets/reaching out.ogg");
		wolfMusic = createMusic("assets/lets play KILL.ogg");

		Mix_PlayMusic(menuMusic, -1);
		//SDL_SetWindowResizable(window, SDL_TRUE);
		//Create renderer and game objects
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

			blackScreen->x = 0;
			blackScreen->y = 0;
			blackScreen->w = width;
			blackScreen->h = height;

			leaveButton = new Button("Leave", "BlackRunters", 250, black, red, 10000, renderer, height / 10, mouseX, mouseY, buttons, currentButtons);
			leaveButton->x = width / 2;
			leaveButton->y = height / 2;

			progressText = new TextArea("Use WASD to turn and move, with space preventing movement but can still turn. Use O to poke and P to swing. Press Enter to progress. Press X to skip a scene. Press Esc to pause/unpause.", "BlackRunters", 250, black, yellow, 3000, renderer, height / 4);
			progressText->x = width - progressText->width / 2;
			progressText->y = height - progressText->height / 2;

			titleText = new TextArea("Life of Sheep", "BlackRunters", 250, blue, green, 3000, renderer, height / 6);
			titleText->x = width / 2;
			titleText->y = height / 2;

			upgradeText = new TextArea("Use the buttons below Gold to upgrade poke speed, swing damage, and maybe the cooldown of something else. Press B to reset gold and upgrades", "BlackRunters", 250, black, yellow, 3000, renderer, height / 3);
			upgradeText->y = height - upgradeText->height / 2;

			goldText = new TextArea("", "BlackRunters", 250, yellow, black, 10000, renderer, height / 10);
			updateGold(level);

			upgradePoke = new Button("", "BlackRunters", 250, black, yellow, 10000, renderer, height / 10, mouseX, mouseY, buttons, currentButtons);
			upgradePoke->y = goldText->height + upgradePoke->height / 2;
			updatePoke(0);

			upgradeSwing = new Button("", "BlackRunters", 250, black, yellow, 10000, renderer, height / 10, mouseX, mouseY, buttons, currentButtons);
			upgradeSwing->y = upgradePoke->y + upgradeSwing->height;
			updateSwing(0);

			upgradeFling = new Button("", "BlackRunters", 250, black, yellow, 10000, renderer, height / 10, mouseX, mouseY, buttons, currentButtons);
			upgradeFling->y = upgradeSwing->y + upgradeFling->height;
			updateFling(0);

			farmerText = new TextArea("", "BlackRunters", 250, red, 10000, renderer, height / 5);
			farmerText->angle = -10;
			
			farmerShake = (float) height / 100.0;

			sheepBar = new Bar(sheepBaseHealth, sheepBaseHealth, green, red, 0, 0, height, true, renderer);
			sheepBar->updateHeight(battleX);

			enemyBar = new Bar(1, 1, red, blue, width - sheepBar->getHeight(), 0, height, true, renderer);
			enemyBar->updateHeight(battleX);

			sheep = new Sheep("assets/sheep1.png", renderer, height / 10, (float) height / 5000.0, 0.1, sheepBaseHealth, sheepBar);
			sheep->setBounds(edge);
			sheep->addSprite("assets/sheep2.png");

			sheepBar2 = new Bar(1, 1, green, red, 0, 0, sheep->width, false, renderer);

			enemyBar2 = new Bar(1, 1, green, red, 0, 0, sheep->width, false, renderer);

			sword = new Sword("assets/sword.png", renderer, height / 10, sheep);
			flyDistance = height / 2 - sword->height / 2;

			grass = new GameObject("assets/grass.png", renderer, height * 2);
			grass->x = width / 2;
			grass->y = height / 2;
			gDis = 100.0 * (float) grass->height / 826.0;

			bull = new Bull("assets/bull.png", renderer, height / 10, 6000, sword, sheep, enemyBar);
			bull->setBounds(edge);

			dog = new Dog("assets/dog.png", renderer, height / 15, 6000, sword, sheep, enemyBar);
			dog->setBounds(edge);

			chicken = new Chicken("assets/chicken.png", renderer, height / 15, 10000, sword, sheep, enemyBar, edge, yellow, red);
			chicken->setBounds(edge);

			horse = new Horse("assets/horse.png", renderer, height / 7, 9000, sword, sheep, enemyBar);
			horse->setBounds(edge);

			cow = new Cow("assets/cow.png", renderer, height / 10, 1000, sword, sheep, enemyBar, edge);
			cow->x = width / 2;
			cow->y = cow->height * 1.5;
			cow->addSprite("assets/cow1.png");
			cow->maxHealth2 = 15000;
			if (level > 4) {
				cow->phase = 4;
				cow->bar = enemyBar2;
				cow->setSprite(1);
				sheep->bar = sheepBar2;
				sheepBar2->setMax(sheep->maxHealth);
				sheepBar2->setValue(sheep->health);
				sheep->setBounds(10000.0);
				sheep->flyable = true;
				progressText->changeText("NEW: Press I to fling your sword when your body is ready, and then teleport to it by pressing I again");
				progressText->x = width - progressText->width / 2;
				maxUpgrades += 2;
				updatePoke(0);
				updateSwing(0);
				updateFling(0);
			}

			wolf1 = new Wolf("assets/wolf.png", renderer, height / 15, 5000, sword, sheep, NULL);
			wolf2 = new Wolf("assets/wolf.png", renderer, height / 15, 5000, sword, sheep, NULL);
			wolf3 = new Wolf("assets/wolf.png", renderer, height / 15, 5000, sword, sheep, NULL);
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
	for (int i : *buttons) {
		currentButtons->erase(i);
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
		case SDL_WINDOWEVENT:
			break;
		case SDL_MOUSEMOTION:
			x = event.motion.x;
			y = event.motion.y;
			//std::cout << *mouseX << ' ' << *mouseY << std::endl;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (!buttons->contains(event.button.button)) {
				currentButtons->insert(event.button.button);
			}
			buttons->insert(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			buttons->erase(event.button.button);
			break;
		}
	}
}

void Game::update(int frame) {
	//std::cout << *mouseX << ' ' << *mouseY << ' ' << upgradePoke->hovered() << std::endl;
	//std::cout << GameObject::objects.size() << std::endl;
	//std::cout << sheep->x << ' ' << sheep->y << std::endl;
	SDL_SetCursor(SDL_GetDefaultCursor());

	if (currentKeys.contains("Escape")) {
		paused = !paused;
		if (paused) {
			if (Mix_PlayingMusic() == 1) {
				Mix_PauseMusic();
			}
			leaveButton->x = GameObject::globalX + width / 2;
			leaveButton->y = GameObject::globalY + height / 2;
		}
		else {
			if (Mix_PausedMusic() == 1) {
				Mix_ResumeMusic();
			}
		}
	}
	if (paused) {
		if (leaveButton->clicked()) {
			clean();
			exit(0);
		}
		return;
	}

	if (room != "Menu" && room != "Prepare" && !cutScene) {
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
		if ((keys.contains("W") || keys.contains("A") || keys.contains("S") || keys.contains("D")) && !keys.contains("Space")) {
			sheep->move(frame);
		}
		sheep->update(frame);
		if (sheep->health == 0) {
			prepare();
			level -= 1;
		}
		if (cow->phase > 2 && room.starts_with("Level")) {
			if (sheep->x - grass->x > gDis) {
				sheep->x -= gDis;
				sword->x -= gDis;
				for (auto e : Enemy::enemies) {
					e->x -= gDis;
				}
			}
			else if (sheep->x - grass->x < -gDis) {
				sheep->x += gDis;
				sword->x += gDis;
				for (auto e : Enemy::enemies) {
					e->x += gDis;
				}
			}

			if (sheep->y - grass->y > gDis) {
				sheep->y -= gDis;
				sword->y -= gDis;
				for (auto e : Enemy::enemies) {
					e->y -= gDis;
				}
			}
			else if (sheep->y - grass->y < -gDis) {
				sheep->y += gDis;
				sword->y += gDis;
				for (auto e : Enemy::enemies) {
					e->y += gDis;
				}
			}
			GameObject::globalX = sheep->x - width / 2;
			GameObject::globalY = sheep->y - height / 2;
			sheep->posBar();
		}

		//Sword update
		if (sword->flying) {
			//std::cout << hypot(sheep->x - sword->x, sheep->y - sword->y) << std::endl;
			if (currentKeys.contains("I") || hypot(sheep->x - sword->x, sheep->y - sword->y) > flyDistance) {
				sword->flying = false;
				sheep->x = sword->x;
				sheep->y = sword->y;
				GameObject::globalX = sheep->x - width / 2;
				GameObject::globalY = sheep->y - height / 2;
				sheep->resetFlight();
			}
			circles.push_back({int(sheep->x - GameObject::globalX), int(sheep->y - GameObject::globalY), flyDistance + sword->height / 2});
		}
		else {
			sword->sheepify();
		}
		if (!sword->out()) {
			if (keys.contains("O")) {
				//std::cout << "here";
				sword->beginPoke();
			}
			else if (keys.contains("P")) {
				sword->beginSwing();
			}
			else if (currentKeys.contains("I")) {
				if (sheep->flightRecharge == 0) {
					sword->flying = true;
					sword->angle += 270;
				}
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
		if (upgradePoke->clicked()) {
			if (updateGold(-1)) {
				if (updatePoke(1)) {
					sword->updatePokeTime();
				}
				else {
					updateGold(1);
				}
			}
		}
		else if (upgradeSwing->clicked()) {
			if (updateGold(-1)) {
				if (updateSwing(1)) {
					sword->updateSwingDmg();
				}
				else {
					updateGold(1);
				}
			}
		}
		else if (upgradeFling->clicked()) {
			if (updateGold(-1)) {
				if (updateFling(1)) {
					sheep->updateFlingTime();
				}
				else {
					updateGold(1);
				}
			}
		}
		else if (currentKeys.contains("B")) {
			sword->reset();
			sheep->resetFling();
			gold = level;
			swingUpgrades = 0;
			pokeUpgrades = 0;
			flingUpgrades = 0;
			updateGold(0);
			updateSwing(0);
			updatePoke(0);
			updateFling(0);
		}
	}
	if (room.starts_with("Level")) {
		switch (level) {
		case 1:
			if (!bull->damaged(frame)) {
				bull->update(frame);
			}
			else {
				if (cow->ticker == -1) {
					if (cow->wiggle(frame)) {
						cow->ticker = 1;
						Mix_PlayMusic(mooMusic, -1);
					}
				}
				else if (cow->ticker == 1) {
					cow->speak("moo-hoo-hoo, what a surprise [Enter]");
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

				if (keys.contains("X")) {
					cow->setOpacity(0.0);
					prepare();
					updateGold(1);
				}
			}
			break;
		case 2:
			if (!dog->damaged(frame)) {
				dog->update(frame);
			}
			else {
				if (cow->ticker == -1) {
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

				if (keys.contains("X")) {
					cow->setOpacity(0.0);
					prepare();
					updateGold(1);
				}
			}
			break;
		case 3:
			if (!chicken->damaged(frame)) {
				chicken->update(frame);
			}
			else {
				if (cow->ticker == -1) {
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

				if (keys.contains("X")) {
					cow->setOpacity(0.0);
					prepare();
					updateGold(1);
				}
			}
			break;
		case 4:
			if (!horse->damaged(frame)) {
				horse->update(frame);
			}
			else {
				if (cow->ticker == -1) {
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

				if (keys.contains("X")) {
					cow->setOpacity(0.0);
					prepare();
					updateGold(1);
				}
			}
			break;
		case 5:
			switch (cow->phase) {
			case 0:
				if (currentKeys.contains("Return")) {
					cutSceneFrame++;
				}
				switch (cutSceneFrame) {
				case 0:
					cow->speak("Welcome sheep");
					break;
				case 1:
					cow->speak("you have made it");
					break;
				case 2:
					cow->speak("to the edge of the farm.");
					break;
				case 3:
					cow->speak("Many have tried to escape here.");
					break;
				case 4:
					cow->speak("None have succeeded.");
					break;
				case 5:
					cow->lookAt(sheep);
					cow->speak("You think you're differnt?");
					break;
				case 6:
					cow->speak("You think you're special?");
					break;
				case 7:
					cow->speak("What hopes do you have against the farmer?");
					break;
				case 8:
					cow->lookAt(sheep);
					cow->angle += 180;
					cow->flip = 2 - cow->flip;
					cow->speak("...");
					break;
				case 9:
					cow->lookAt(sheep);
					cow->speak("What hopes do you have against me?");
					break;
				case 10:
					cow->phase++;
					cutScene = false;
					cutSceneFrame = 0;
					Mix_PlayMusic(cow1Music, 1);
					break;
				}

				if (keys.contains("X")) {
					cow->phase++;
					cutScene = false;
					cutSceneFrame = 0;
					Mix_PlayMusic(cow1Music, 1);
				}
				break;
			case 1:
				if (!cow->damaged(frame)) {
					cow->update(frame);
				}
				else {
					cow->phase++;
					cutScene = true;
					cow->setOpacity(1.0);
					sword->x += height * 2;
					cow->damaged(frame);
					cow->boltsClear();
					Mix_FadeOutMusic(1000);
				}
				break;
			case 2:
				switch (cutSceneFrame) {
				case 0:
					if (cow->wiggle(-frame)) {
						cutSceneFrame++;
					}
					break;
				case 1:
					cow->x = width / 2;
					cow->y = height / 2;
					sword->prepare();
					cutSceneFrame++;
					break;
				case 2:
					if (cow->wiggle(frame)) {
						cutSceneFrame++;
					}
					break;
				case 3:
					cow->lookAt(sheep);
					sheep->lookAt(cow);
					Mix_PlayMusic(handMusic, -1);
					cutSceneFrame++;
					break;
				case 4:
					cow->speak("*huff* *huff*");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 5:
					cow->speak("why?");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 6:
					cow->speak("WHY!?");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 7:
					cow->speak("DO YOU REALIZE WHAT YOU'VE DONE!?");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 8:
					sheep->flip = 0;
					sheep->angle = 0;
					cow->flip = 0;
					cow->angle = 0;
					cutSceneFrame++;
					break;
				case 9:
					farmerSpeak("COW...");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 10:
					GameObject::globalY = 0.0;
					farmerSpeak("IT'S TIME...");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 11:
					GameObject::globalY = 0.0;
					for (int i = 0; i < 8; i++) {
						Hand* h = createHand(height / 30.0, 1);
						h->x = radius * cos(QPI * i) + cow->x;
						h->y = radius * sin(QPI * i) + cow->y;
						h->lookAt(cow);
					}
					cutSceneFrame++;
					break;
				case 12:
					cow->speak("no...");
					for (auto h : hands) {
						h->moveTowards(cow, 3.0 * frame);
					}
					if (currentKeys.contains("Return")) {
						Mix_FadeOutMusic(1000);
						cutSceneFrame++;
					}
					break;
				case 13:
					cow->angle = 15;
					cow->speak("NO...");
					for (auto h : hands) {
						h->moveTowards(cow, 3.0 * frame);
					}
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 14:
					Mix_PlayMusic(cow2Music, -1);
					cow->setSprite(1);
					cow->setOpacity(1);
					ticker = 0.0;
					cutSceneFrame++;
					break;
				case 15:
					ticker += float(frame) / 500.0;
					for (double i = remainder(ticker, 1.0); i < ticker; i++) {
						circles.push_back({ width / 2, height / 2, int(i * float(height) / 5.0) });
					}
					if (ticker > 5.0) {
						ticker -= 1.0;
					}
					edge += (float)frame / 25000.0;
					battleX = (1 - edge) / 2.0 * width;
					battleY = (1 - edge) / 2.0 * height;
					sheepBar->updatePos(battleX - sheepBar->getHeight(), 0);
					enemyBar->updatePos(battleX + edge * width, 0);
					cow->speak("NO!!!");
					for (auto h : hands) {
						h->moveTowards(cow, -5.0 * frame);
					}
					if (edge > 1.0) {
						cow->lookAt(sheep);
						if (currentKeys.contains("Return")) {
							cutSceneFrame++;
						}
					}
					break;
				case 16:
					for (auto h : hands) {
						destroyHand(h);
					}
					hands.clear();
					cutSceneFrame++;
					break;
				case 17:
					cow->speak("I AM COW");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 18:
					cow->speak("AND I WILL NOT BE FORGOTTEN");
					if (currentKeys.contains("Return")) {
						cow->phase++;
						cow->ticker = 0;
						cutScene = false;
						cutSceneFrame = 0;
						cow->health = cow->maxHealth2;
						cow->bar = enemyBar2;
						enemyBar2->setMax(cow->health);
						enemyBar2->setValue(cow->health);
						sheep->bar = sheepBar2;
						sheepBar2->setMax(sheep->maxHealth);
						sheepBar2->setValue(sheep->health);
						sheep->setBounds(10000.0);
					}
					break;
				}

				if (keys.contains("X")) {
					for (auto h : hands) {
						destroyHand(h);
					}
					hands.clear();
					cow->phase++;
					cow->ticker = 0;
					cutScene = false;
					cutSceneFrame = 0;
					cow->health = cow->maxHealth2;
					cow->bar = enemyBar2;
					enemyBar2->setMax(cow->health);
					enemyBar2->setValue(cow->health);
					cow->setSprite(1);
					sheep->bar = sheepBar2;
					sheepBar2->setMax(sheep->maxHealth);
					sheepBar2->setValue(sheep->health);
					sheep->setBounds(10000.0);
					Mix_PlayMusic(cow2Music, -1);
				}
				break;
			case 3:
				if (!cow->damaged(frame)) {
					cow->update(frame);
					ticker += (float)frame / 1000.0;
					if (ticker > 2.0) {
						int r = rand();
						Hand* h = createHand(height / (r % 30 + 15), 1);
						h->x = radius * cos(r) + GameObject::globalX;
						h->y = radius * sin(r) + GameObject::globalY;
						ticker = 0.0;
					}
					int size = hands.size();
					for (int i = 0; i < size; i++) {
						if (hands[i]->moveTowards(cow, frame)) {
							cow->x += (float)(hands[i]->height * frame) / 1000.0;
						}
						hands[i]->damage(hands[i]->height * 75 / height);
						hands[i]->damage(hands[i]->height * 75 / height, cow);
						if (hands[i]->collided(sword) && sword->swordOut) {
							destroyHand(hands[i]);
							hands.erase(std::next(hands.begin(), i));
							i--;
							size--;
							continue;
						}
						for (auto bolt : cow->bolts) {
							if (bolt->collided(hands[i])) {
								destroyHand(hands[i]);
								hands.erase(std::next(hands.begin(), i));
								i--;
								size--;
								break;
							}
						}
					}
				}
				else {
					cow->phase++;
					cutScene = true;
					cow->setOpacity(1.0);
					sword->x += height * 2;
					cow->damaged(frame);
					Mix_FadeOutMusic(3000);
				}
				break;
			case 4:
				switch (cutSceneFrame) {
				case 0:
					GameObject::globalX = cow->x - width / 2;
					GameObject::globalY = cow->y - height / 2;
					cow->setOpacity(1.0);
					if (true) {
						Hand* h = createHand(height / 5, 1);
						h->x = width + h->width / 2 + GameObject::globalX;
						h->y = height / 2 + GameObject::globalY;
					}
					cow->boltsClear();
					cutSceneFrame++;
					break;
				case 1:
					for (auto h : hands) {
						if (h->moveTowards(cow, frame)) {
							cow->x += (float)(h->height * frame) / 1000.0;
						}
					}
					cow->speak("NOOOO");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 2:
					for (auto h : hands) {
						if (h->moveTowards(cow, frame)) {
							cow->x += (float)(h->height * frame) / 1000.0;
						}
					}
					cow->speak("IT CANNOT BE");
					if (currentKeys.contains("Return") && Mix_PlayingMusic() == 0) {
						ticker = 1.0;
						Mix_PlayMusic(handMusic, -1);
						cutSceneFrame++;
					}
					break;
				case 3:
					if (cow->x - GameObject::globalX < width + cow->width / 2) {
						if (ticker > 0.0) {
							cow->setOpacity(ticker);
							ticker -= (float)frame / 1000.0;
						}
						else {
							cow->setOpacity(0.0);
							Mix_FadeOutMusic(10000);
							cutSceneFrame++;
						}
					}
					else {
						Mix_FadeOutMusic(10000);
						cutSceneFrame++;
					}
					break;
				case 4:
					cutScene = false;
					for (auto h : hands) {
						if (h->moveTowards(sheep, frame * h->height * 20.0 / height)) {
							sheep->x += (float)(h->height * frame) / 10000.0;
							h->damage(h->height * 200 / height);
						}
						if (h->collided(sword) && sword->swordOut) {
							h->setKnockback(-(float)sword->height * 25.0 * cos(h->radians), -(float)sword->height * 25.0 * sin(h->radians), 50);
						}
						h->knockback(frame);
					}
					if (Mix_PlayingMusic() == 0) {
						cutSceneFrame = 0;
						sheep->flyable = true;
						room = "Menu";
						Mix_PlayMusic(menuMusic, -1);
						GameObject::globalX = 0.0;
						GameObject::globalY = 0.0;				
						updateGold(1);
						for (auto h : hands) {
							destroyHand(h);
						}
						hands.clear();
						progressText->changeText("NEW: Press I to fling your sword when your body is ready, and then teleport to it by pressing I again");
						progressText->x = width - progressText->width / 2;
						maxUpgrades += 2;
						updatePoke(0);
						updateSwing(0);
						updateFling(0);
					}
					break;
				}
				break;
			}
			break;
		case 6:
			switch (Wolf::phase) {
			case 0:
				switch (cutSceneFrame) {
				case 0:
					cutScene = true;
					wolf1->x = width / 2 - height / 3;
					wolf1->y = -height / 4;
					wolf1->angle = 270;
					wolf2->x = width / 2;
					wolf2->y = -height / 4;
					wolf2->angle = 270;
					wolf3->x = width / 2 + height / 3;
					wolf3->angle = 270;
					wolf3->y = -height / 4;
					ticker = 0.0;
					cutSceneFrame++;
					break;
				case 1:
					wolf1->turnTowards(sheep, frame);
					wolf3->turnTowards(sheep, frame);
					wolf1->move(frame);
					wolf2->move(frame);
					wolf3->move(frame);
					ticker += float(frame) / 1000.0;
					if (ticker > 1.5) {
						sheep->lookAt(wolf2);
						wolf3->lookAt(wolf2);
						//std::cout << wolf1->x << ' ' << wolf1->y;
						cutSceneFrame++;
					}
					break;
				case 2:
					wolf2->speak("well well well boys look what we got here");
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 3:
					wolf2->speak("looks like meat is back on the menu boys");
					if (currentKeys.contains("Return")) {
						sheep->lookAt(wolf3);
						cutSceneFrame++;
					}
					break;
				case 4:
					wolf3->speak("can I be on the menu?");
					if (currentKeys.contains("Return")) {
						wolf2->lookAt(wolf3);
						cutSceneFrame++;
					}
					break;
				case 5:
					wolf2->speak("...");
					if (currentKeys.contains("Return")) {
						wolf2->lookAt(sheep);
						cutSceneFrame++;
					}
					break;
				case 6:
					wolf2->speak("no");
					if (currentKeys.contains("Return")) {
						wolf3->lookAt(sheep);
						Mix_PlayMusic(wolfMusic, -1);
						Wolf::phase++;
						cutScene = false;
						cutSceneFrame = 0;
						sheep->angle = 0;
					}
					break;
				}
				if (cutSceneFrame > 1 && keys.contains("X")) {
					wolf2->lookAt(sheep);
					wolf3->lookAt(sheep);
					Mix_PlayMusic(wolfMusic, -1);
					Wolf::phase++;
					cutScene = false;
					cutSceneFrame = 0;
					sheep->angle = 0;
				}
				break;
			case 1:
				if (!(wolf1->damaged(frame) || wolf2->damaged(frame) || wolf3->damaged(frame))) {
					wolf1->update1(frame);
					wolf2->update2(frame);
					wolf3->update3(frame);
				}
				break;
			}
			break;
		}
	}
}


void Game::render() {
	//std::cout << room << std::endl;
	grass->render();
	if (room != "Menu") {
		sheep->render();
		//std::cout << sheep->inArea(*mouseX, *mouseY) << std::endl;
		if (room != "Prepare") {
			sword->renderSwing();
		}
	}
	if (room == "Menu") {
		progressText->render();
		titleText->render();
	}
	else if (room == "Prepare") {
		progressText->render();
		upgradeText->render();
		goldText->render();
		upgradePoke->render();
		upgradeSwing->render();
		if (level > 4) {
			upgradeFling->render();
		}
	}
	if (room.starts_with("Level")) {
		switch (level) {
		case 1:
			bull->render();
			break;
		case 2:
			dog->render();
			break;
		case 3:
			chicken->render();
			break;
		case 4:
			horse->render();
			break;
		case 5:
			if (cow->phase < 3) {
				enemyBar->render();
				//render edge of battlefield
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderDrawLine(renderer, battleX, battleY, battleX + edge * width, battleY);
				SDL_RenderDrawLine(renderer, battleX + edge * width, battleY + edge * height, battleX + edge * width, battleY);
				SDL_RenderDrawLine(renderer, battleX + edge * width, battleY + edge * height, battleX, battleY + edge * height);
				SDL_RenderDrawLine(renderer, battleX, battleY, battleX, battleY + edge * height);
			}
			cow->render();
			break;
		case 6:
			wolf1->render();
			wolf2->render();
			wolf3->render();
			break;
		}
		if (level < 5) {
			cow->render();
			enemyBar->render();
			//render edge of battlefield
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(renderer, battleX, battleY, battleX + edge * width, battleY);
			SDL_RenderDrawLine(renderer, battleX + edge * width, battleY + edge * height, battleX + edge * width, battleY);
			SDL_RenderDrawLine(renderer, battleX + edge * width, battleY + edge * height, battleX, battleY + edge * height);
			SDL_RenderDrawLine(renderer, battleX, battleY, battleX, battleY + edge * height);
		}
		if (cow->phase < 3 || !cutScene) {
			sheep->bar->render();
		}
	}
	if (farmerText->text != "") {
		farmerText->render();
		farmerText->text = "";
	}
	for (auto h : hands) {
		h->render();
	}
	if (paused) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
		SDL_RenderFillRect(renderer, blackScreen);
		leaveButton->render();
	}
	for (std::vector<int> c : circles) {
		drawCircle(c[0], c[1], c[2]);
	}
	circles.clear();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	for (auto m : music) {
		Mix_FreeMusic(m);
	}
	for (auto o = GameObject::objects.begin(); o != GameObject::objects.end(); ) {
		(*o)->clear();
		//std::cout << GameObject::objects.size() << std::endl;
		o = GameObject::objects.begin();
	}
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
	Mix_FadeOutMusic(2000);
	room = "Prepare";
	GameObject::globalX = 0.0;
	GameObject::globalY = 0.0;
	grass->x = width / 2;
	grass->y = height / 2;
	sheep->x = 3 * sheep->width;
	sheep->y = 3 * sheep->height;
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
	sheep->resetFlight();
	Mix_HaltMusic();
	sheep->width /= 4;
	sheep->height /= 4;
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
		break;
	case 5:
		edge = originalEdge;
		cow->bar = enemyBar;
		sheep->bar = sheepBar;
		sheep->setBounds(edge);
		battleX = (1 - edge) / 2.0 * width;
		battleY = (1 - edge) / 2.0 * height;
		sheepBar->updatePos(0, 0);
		enemyBar->updatePos(width - enemyBar->getHeight(), 0);
		cow->prepare();
		cutScene = true;
		for (auto h : hands) {
			destroyHand(h);
		}
		hands.clear();
	case 6:
		wolf1->prepare();
		wolf2->prepare();
		wolf3->prepare();
		Wolf::phase = 0;
	}
	sheep->prepare(level < 6);
	sword->prepare();
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

bool Game::updatePoke(int amount) {
	pokeUpgrades += amount;
	if (pokeUpgrades <= maxUpgrades) {
		upgradePoke->changeText("Poke: " + std::to_string(pokeUpgrades) + "/" + std::to_string(maxUpgrades));
		upgradePoke->x = width - upgradePoke->width / 2;
	}
	else {
		pokeUpgrades -= amount;
		return false;
	}
	return true;
}

bool Game::updateSwing(int amount) {
	swingUpgrades += amount;
	if (swingUpgrades <= maxUpgrades) {
		upgradeSwing->changeText("Swing: " + std::to_string(swingUpgrades) + "/" + std::to_string(maxUpgrades));
		upgradeSwing->x = width - upgradeSwing->width / 2;
	}
	else {
		swingUpgrades -= amount;
		return false;
	}
	return true;
}

bool Game::updateFling(int amount) {
	flingUpgrades += amount;
	if (flingUpgrades <= maxUpgrades) {
		upgradeFling->changeText("Fling: " + std::to_string(flingUpgrades) + "/" + std::to_string(maxUpgrades));
		upgradeFling->x = width - upgradeFling->width / 2;
	}
	else {
		flingUpgrades -= amount;
		return false;
	}
	return true;
}

Mix_Music* Game::createMusic(const char* file) {
	Mix_Music* m = Mix_LoadMUS(file);
	if (m == NULL) {
		std::cout << SDL_GetError() << std::endl;
	}
	music.push_back(m);
	return m;
}

void Game::farmerSpeak(std::string text) {
	farmerText->changeText(text);
	GameObject::globalY += farmerShake;
	farmerShake *= -1.0;
	farmerText->x = width - farmerText->width / 2 + GameObject::globalX;
	farmerText->y = height / 2 + GameObject::globalY;
}

//also puts hand in hands
Hand* Game::createHand(uint16_t height, int health) {
	Bar* b = new Bar(health, health, green, red, 0, 0, sheep->width, false, renderer);
	Hand* h = new Hand("assets/hand.png", renderer, height, health, sword, sheep, b);
	hands.push_back(h);
	return h;
}

void Game::destroyHand(Hand* hand) {
	delete hand->bar;
	hand->clear();
	delete hand;
}

void Game::drawCircle(int x, int y, int radius)
{
	int diameter = (radius * 2);

	int dx = (radius - 1);
	int dy = 0;
	int tx = 1;
	int ty = 1;
	int error = (tx - diameter);

	while (dx >= dy)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, x + dx, y - dy);
		SDL_RenderDrawPoint(renderer, x + dx, y + dy);
		SDL_RenderDrawPoint(renderer, x - dx, y - dy);
		SDL_RenderDrawPoint(renderer, x - dx, y + dy);
		SDL_RenderDrawPoint(renderer, x + dy, y - dx);
		SDL_RenderDrawPoint(renderer, x + dy, y + dx);
		SDL_RenderDrawPoint(renderer, x - dy, y - dx);
		SDL_RenderDrawPoint(renderer, x - dy, y + dx);

		if (error <= 0)
		{
			++dy;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--dx;
			tx += 2;
			error += (tx - diameter);
		}
	}
}
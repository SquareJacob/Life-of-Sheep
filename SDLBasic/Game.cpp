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

Sheep* sheep;
const int sheepBaseHealth = 300;
int gold = 0;

Sword* sword;

SDL_Color red = { 191, 33, 47 };
SDL_Color black = { 0, 0, 0 };
SDL_Color green = { 0, 111, 60 };
SDL_Color blue = { 37, 75, 150 };
SDL_Color yellow = { 250, 167, 63 };

TextArea* progressText;
TextArea* titleText;
TextArea* upgradeText;
TextArea* goldText;


TextArea* farmerText;
double farmerShake;

Bull* bull;
Dog* dog;
Chicken* chicken;
Horse* horse;
Cow* cow;

std::vector<Mix_Music*> music;
Mix_Music* menuMusic;
Mix_Music* bullMusic;
Mix_Music* mooMusic;
Mix_Music* dogMusic;
Mix_Music* chickenMusic;
Mix_Music* horseMusic;
Mix_Music* cow1Music;
Mix_Music* cow2Music;

Bar* sheepBar;
Bar* sheepBar2;
Bar* enemyBar;
Bar* enemyBar2;

GameObject* grass;
double gDis;

bool cutScene = false;
uint8_t cutSceneFrame = 0;

std::vector<Hand*> hands;

Game::Game(const char* title, bool fullscreen) {
	room = "Menu";
	level = 4;
	edge = originalEdge;
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
		menuMusic = createMusic("assets/a sheepish menu.ogg");
		bullMusic = createMusic("assets/CHARGE!!.ogg");
		mooMusic = createMusic("assets/moo.ogg");
		dogMusic = createMusic("assets/lets play fetch.ogg");
		chickenMusic = createMusic("assets/cock.ogg");
		horseMusic = createMusic("assets/horsin' around.ogg");
		cow1Music = createMusic("assets/a magic cow.ogg");
		cow2Music = createMusic("assets/YET TO BE FORGOTTEN.ogg");
		Mix_PlayMusic(menuMusic, -1);
		//SDL_SetWindowResizable(window, SDL_TRUE);
		//Create renderer and game objects
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			progressText = new TextArea("Use WASD to move, hold down to space to prevent movement. Use O to poke and P to swing. Press Enter to progress. Press X to skip a scene.", "BlackRunters", 250, black, yellow, 3000, renderer, height / 4);
			progressText->x = width - progressText->width / 2;
			progressText->y = height - progressText->height / 2;

			titleText = new TextArea("Life of Sheep", "BlackRunters", 250, blue, green, 3000, renderer, height / 6);
			titleText->x = width / 2;
			titleText->y = height / 2;

			upgradeText = new TextArea("Press N to upgrade poke speed, M to upgrade swing damage, each costing 1 gold. Press B to reset gold and upgrades", "BlackRunters", 250, black, yellow, 2000, renderer, height / 3);
			upgradeText->y = height - upgradeText->height / 2;

			goldText = new TextArea("Gold :" + std::to_string(gold), "BlackRunters", 250, yellow, black, 10000, renderer, height / 10);
			updateGold(level);

			farmerText = new TextArea("", "BlackRunters", 250, red, 10000, renderer, height / 5);
			farmerText->angle = -10;
			
			farmerShake = (float) height / 100.0;

			sheepBar = new Bar(sheepBaseHealth, sheepBaseHealth, green, red, 0, 0, height, true, renderer);
			sheepBar->updateHeight(battleX);

			enemyBar = new Bar(1, 1, red, blue, width - sheepBar->getHeight(), 0, height, true, renderer);
			enemyBar->updateHeight(battleX);

			sheep = new Sheep("assets/sheep1.png", renderer, height / 10, (float) height / 5000.0, 0.1, sheepBaseHealth, sheepBar);
			sheep->setBounds(edge);

			sheepBar2 = new Bar(1, 1, green, red, 0, 0, sheep->width, false, renderer);

			enemyBar2 = new Bar(1, 1, green, red, 0, 0, sheep->width, false, renderer);

			sword = new Sword("assets/sword.png", renderer, height / 10, sheep);

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

			horse = new Horse("assets/horse.png", renderer, height / 7, 6000, sword, sheep, enemyBar);
			horse->setBounds(edge);

			cow = new Cow("assets/cow.png", renderer, height / 10, 1000, sword, sheep, enemyBar, edge);
			cow->x = width / 2;
			cow->y = cow->height * 1.5;
			cow->addSprite("assets/cow1.png");

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
		case SDL_WINDOWEVENT:
			break;
		}
	}
}

void Game::update(int frame) {
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
			sheep->posBar();
			GameObject::globalX = sheep->x - width / 2;
			GameObject::globalY = sheep->y - height / 2;
			if (sheep->x - grass->x > gDis) {
				grass->x += gDis;
			}
			else if (sheep->x - grass->x < -gDis) {
				grass->x -= gDis;
			}

			if (sheep->y - grass->y > gDis) {
				grass->y += gDis;
			}
			else if (sheep->y - grass->y < -gDis) {
				grass->y -= gDis;
			}
		}

		//Sword update
		sword->sheepify();
		if (!sword->out()) {
			if (keys.contains("O")) {
				//std::cout << "here";
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
				sword->updatePokeTime();
			}
		}
		else if (currentKeys.contains("M")) {
			if (updateGold(-1)) {
				sword->updateSwingDmg();
			}
		}
		else if (currentKeys.contains("B")) {
			sword->reset();
			gold = level;
			updateGold(0);
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
					Mix_HaltMusic();
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
					for (int i = 0; i < 5; i++) {
						hands.push_back(createHand(height / 30.0, 1));
						hands[i]->x =  width + hands[i]->width;
						hands[i]->y = (float)height * (0.1 + 0.2 * (float) i);
						hands[i]->angle = 180;
					}
					cutSceneFrame++;
					break;
				case 12:
					cow->speak("no...");
					for (auto h : hands) {
						h->moveTowards(cow, frame);
					}
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 13:
					cow->angle = 15;
					cow->speak("NO...");
					for (auto h : hands) {
						h->moveTowards(cow, frame);
					}
					if (currentKeys.contains("Return")) {
						cutSceneFrame++;
					}
					break;
				case 14:
					Mix_PlayMusic(cow2Music, -1);
					cow->setSprite(1);
					cutSceneFrame++;
					break;
				case 15:
					edge += (float)frame / 25000.0;
					battleX = (1 - edge) / 2.0 * width;
					battleY = (1 - edge) / 2.0 * height;
					sheepBar->updatePos(battleX - sheepBar->getHeight(), 0);
					enemyBar->updatePos(battleX + edge * width, 0);
					cow->speak("NO!!!");
					for (auto h : hands) {
						h->x += (float)(frame * height) / 1000.0;
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
					cow->speak("AND I WILL NOT BE FORGOTEEN");
					if (currentKeys.contains("Return")) {
						cow->phase++;
						cutScene = false;
						cutSceneFrame = 0;
						cow->health = 15000;
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
					cutScene = false;
					cutSceneFrame = 0;
					cow->health = 15000;
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
					if (rand() % 100 == 99) {
						int he = rand() % 30 + 15;
						Hand* h = createHand(height / (he), 1);
						h->x = width + GameObject::globalX + h->width * 2;
						h->y = GameObject::globalY + height * (rand() % 1000) / 1000;
						hands.push_back(h);
					}
					int size = hands.size();
					for (int i = 0; i < size; i++) {
						if (hands[i]->moveTowards(cow, frame)) {
							cow->x += (float)(hands[i]->height * frame) / 1000.0;
						}
						hands[i]->damage(hands[i]->height * 75 / height);
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
					Mix_HaltMusic();
				}
				break;
			case 4:
				break;
			}
		}
	}
}


void Game::render() {
	grass->render();
	if (room == "Menu") {
		progressText->render();
		titleText->render();
	}
	else if (room == "Prepare") {
		progressText->render();
		upgradeText->render();
		goldText->render();
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
		}
		if (level < 5) {
			cow->render();
			//render edge of battlefield
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(renderer, battleX, battleY, battleX + edge * width, battleY);
			SDL_RenderDrawLine(renderer, battleX + edge * width, battleY + edge * height, battleX + edge * width, battleY);
			SDL_RenderDrawLine(renderer, battleX + edge * width, battleY + edge * height, battleX, battleY + edge * height);
			SDL_RenderDrawLine(renderer, battleX, battleY, battleX, battleY + edge * height);
		}
		sheep->bar->render();
	}
	if (room != "Menu") {
		sheep->render();
		if (room != "Prepare") {
			sword->renderSwing();
		}
	}
	if (farmerText->text != "") {
		farmerText->render();
		farmerText->text = "";
	}
	for (auto h : hands) {
		h->render();
	}
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	for (auto m : music) {
		Mix_FreeMusic(m);
	}
	for (auto o : GameObject::objects) {
		o->clear();
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
	Mix_FadeOutMusic(1000);
	room = "Prepare";
	GameObject::globalX = 0.0;
	GameObject::globalY = 0.0;
	grass->x = width / 2;
	grass->y = height / 2;
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
		cow->prepare();
		cutScene = true;
		for (auto h : hands) {
			destroyHand(h);
		}
		hands.clear();
	}
	sheep->prepare();
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


Hand* Game::createHand(uint16_t height, int health) {
	Bar* b = new Bar(health, health, green, red, 0, 0, sheep->width, false, renderer);
	Hand* h = new Hand("assets/hand.png", renderer, height, health, sword, sheep, b);
	return h;
}

void Game::destroyHand(Hand* hand) {
	delete hand->bar;
	hand->clear();
	delete hand;
}
#include "GameObject.h"
#include "Sprite.h";

void GameObject::objectize(uint16_t height) {
	sprites.push_back(sprite);
	angle = 0.0;
	radians = 0.0;
	flip = 0;
	width = sprite->getPorportion() * height;
	this->height = height;
	cWidth = width;
	cHeight = height;
	x = width * 0.5;
	y = height * 0.5;
	SDL_GetRendererOutputSize(sprite->renderer, &gameWidth, &gameHeight);
	setBounds();
}

GameObject::GameObject(SDL_Surface* surface, SDL_Renderer* renderer, uint16_t height) {
	sprite = new Sprite(surface, renderer);
	objectize(height);
}

GameObject::GameObject(const char* spriteFile, SDL_Renderer* renderer, uint16_t height) {
	sprite = new Sprite(spriteFile, renderer);
	objectize(height);
}

GameObject::GameObject(std::string text, std::string font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height) {
	sprite = new Sprite(text, font, size, fg, bg, wrapLength, renderer);
	objectize(height);
}

GameObject::GameObject(std::string text, std::string font, int size, SDL_Color fg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height) {
	sprite = new Sprite(text, font, size, fg, wrapLength, renderer);
	objectize(height);
}

GameObject::~GameObject() {}

void GameObject::render() {
	sprite->place(x - width / 2, y - height / 2, width, height, angle, flip);
}

double GameObject::setRadians() {
	radians = angle * PI / 180.0;
	return radians;
}

bool GameObject::shift(double vel) {
	slide(vel);
	return bound();
}

void GameObject::slide(double vel) {
	x += vel * cos(setRadians()) * (1 - 4 * flip + 2 * flip * flip);
	y -= vel * sin(radians) * (1 - 4 * flip + 2 * flip * flip);
}

void GameObject::tilt(double vel) {
	angle += vel;
	angle = std::fmod(angle, 360.0);
}

void GameObject::setBounds() {
	lowerX = width / 2;
	lowerY = height / 2;
	upperX = gameWidth - width / 2;
	upperY = gameHeight - height / 2;
}

void GameObject::setBounds(double edge) {
	lowerX = width / 2 + (1 - edge) / 2.0 * gameWidth;
	lowerY = height / 2 + (1 - edge) / 2.0 * gameHeight;
	upperX = gameWidth - width / 2 - (1 - edge) / 2.0 * gameWidth;
	upperY = gameHeight - height / 2 - (1 - edge) / 2.0 * gameHeight;
}

bool GameObject::bound() {
	bool result = false;
	if (x < lowerX) {
		x = lowerX;
		result = true;
	}
	else if (x > upperX) {
		x = upperX;
		result = true;
	}
	if (y < lowerY) {
		y = lowerY;
		result = true;
	}
	else if (y > upperY) {
		y = upperY;
		result = true;
	}
	return result;
}

void GameObject::cornerize(std::vector<double>& corners) {
	c = cos(setRadians());
	s = sin(radians);

	corners[0] = x + cWidth / 2 * c - cHeight / 2 * s;
	corners[1] = y - cWidth / 2 * s - cHeight / 2 * c;
	corners[2] = x - cWidth / 2 * c - cHeight / 2 * s;
	corners[3] = y + cWidth / 2 * s - cHeight / 2 * c;
	corners[4] = x - cWidth / 2 * c + cHeight / 2 * s;
	corners[5] = y + cWidth / 2 * s + cHeight / 2 * c;
	corners[6] = x + cWidth / 2 * c + cHeight / 2 * s;
	corners[7] = y - cWidth / 2 * s + cHeight / 2 * c;
}

void GameObject::renderHitBox(int r, int g, int b) {
	cornerize(corners);
	SDL_SetRenderDrawColor(sprite->renderer, r, g, b, 255);
	SDL_RenderDrawLine(sprite->renderer, corners[0], corners[1], corners[2], corners[3]);
	SDL_RenderDrawLine(sprite->renderer, corners[4], corners[5], corners[2], corners[3]);
	SDL_RenderDrawLine(sprite->renderer, corners[4], corners[5], corners[6], corners[7]);
	SDL_RenderDrawLine(sprite->renderer, corners[0], corners[1], corners[6], corners[7]);
}

bool GameObject::collided(GameObject* object, bool talk) {
	uint8_t intersections = 0;
	int8_t positives = 0;

	uVectors[0] = cos(setRadians());
	uVectors[1] = sin(radians);
	uVectors[2] = cos(object->setRadians());
	uVectors[3] = sin(object->radians);

	object->cornerize(corners);

	//check along axis parallel to this width: https://www.desmos.com/calculator/o3j2g63oco
	for (int i = 0; i < 8; i += 2) {
		distances[i / 2] = (corners[i + 1] - y) * uVectors[1] - (corners[i] - x) * uVectors[0];
		if (abs(distances[i / 2]) < cWidth / 2) {
			intersections++;
			break;
		}
		if (i == 6) {
			for (int j = 0; j < 4; j++) {
				if (distances[j] > 0) {
					positives++;
				}
				else {
					positives--;
				}
			}
			if (positives != 4 && positives != -4) {
				intersections++;
			}
			positives = 0;
		}
	}

	//check along axis parallel to this height: https://www.desmos.com/calculator/65dyflzkti
	for (int i = 0; i < 8; i += 2) {
		distances[i / 2] = (corners[i + 1] - y) * uVectors[0] - (x - corners[i]) * uVectors[1];
		if (abs(distances[i / 2]) < cHeight / 2) {
			intersections++;
			break;
		}
		if (i == 6) {
			for (int j = 0; j < 4; j++) {
				if (distances[j] > 0) {
					positives++;
				}
				else {
					positives--;
				}
			}
			if (positives != 4 && positives != -4) {
				intersections++;
			}
			positives = 0;
		}
	}

	cornerize(corners);

	//check along axis parallel to object width: https://www.desmos.com/calculator/o3j2g63oco
	for (int i = 0; i < 8; i += 2) {
		distances[i / 2] = (corners[i + 1] - object->y) * uVectors[3] - (corners[i] - object->x) * uVectors[2];
		if (abs(distances[i / 2]) < object->cWidth / 2) {
			intersections++;
			break;
		}
		if (i == 6) {
			for (int j = 0; j < 4; j++) {
				if (distances[j] > 0) {
					positives++;
				}
				else {
					positives--;
				}
			}
			if (positives != 4 && positives != -4) {
				intersections++;
			}
			positives = 0;
		}
	}

	//check along axis parallel to object height: https://www.desmos.com/calculator/65dyflzkti	
	for (int i = 0; i < 8; i += 2) {
		distances[i / 2] = (corners[i + 1] - object->y) * uVectors[2] - (object->x - corners[i]) * uVectors[3];
		if (abs(distances[i / 2]) < object->cHeight / 2) {
			intersections++;
			break;
		}
		if (i == 6) {
			for (int j = 0; j < 4; j++) {
				if (distances[j] > 0) {
					positives++;
				}
				else {
					positives--;
				}
			}
			if (positives != 4 && positives != -4) {
				intersections++;
			}
			positives = 0;
		}
	}
	return intersections == 4;
}

void GameObject::addSprite(const char* file) {
	sprites.push_back(new Sprite(file, sprite->renderer));
}

void GameObject::setSprite(uint8_t number) {
	sprite = sprites[number];
	width = sprite->getPorportion() * height;
}

void GameObject::setKnockback(double x, double y, double time) {
	knockX = x;
	knockY = y;
	knockTime = time;
}

void GameObject::knockback(double frame) {
	if (knockTime > 0.0) {
		x += knockX * frame / 1000.0;
		y += knockY * frame / 1000.0;
		if (bound()) {
			knockTime = 0.0;
		}
		else {
			knockTime -= frame;
			knockTime = knockTime < 0.0 ? 0.0 : knockTime;
		}
	}
}

double GameObject::c = 0.0;
double GameObject::s = 0.0;
std::vector<double> GameObject::corners(8);
std::vector<double> GameObject::uVectors(4);
std::vector<double> GameObject::distances(4);


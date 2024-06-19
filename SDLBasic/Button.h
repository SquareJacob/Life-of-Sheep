#pragma once
#include "TextArea.h"
class Button :
    public TextArea
{
public:
    Button(std::string text, std::string font, int size, SDL_Color fg, SDL_Color bg, Uint32 wrapLength, SDL_Renderer* renderer, uint16_t height, int* mX, int* mY, std::set<int>* buttons, std::set<int>* cButtons);
    bool hovered();
    bool clicked();
    void render();
protected:
    int* mouseX;
    int* mouseY;
    std::set<int>* buttons;
    std::set<int>* currentButtons;
};


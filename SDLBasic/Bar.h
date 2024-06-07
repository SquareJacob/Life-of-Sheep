#pragma once
#include "GameObject.h"
class Bar
{
public:
    //(x,y) specifies topleft corner
    Bar(Uint32 iValue, Uint32 mValue, SDL_Color fg, SDL_Color bg, Uint16 x, Uint16 y, Uint16 length, bool vertical, SDL_Renderer* renderer);
    //updates physical bar
    void setValue(Uint32 value);
    //updates physical bar
    void setMax(Uint32 value);
    void render();
    Uint16 height = 12;
protected:
    void createRects();
    void updateBar();
    Uint32 iValue, mValue;
    SDL_Color fColor, bColor;
    Uint16 length, x, y;
    bool vertical;
    SDL_Rect fg, bg;
    SDL_Renderer* renderer;
};


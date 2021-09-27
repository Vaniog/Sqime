#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

class Button // its button
{
public:
    Button(int xIn, int yIn, int startDownloadingFromX, int startDownloadingFromY, int textureW, int textureH, long double widthIn, long double heightIn, string buttonTexPlace);

    int buttonDisplayAndCheck(RenderWindow *window, int xIn, int yIn);
    void buttonModeSet(int mode); // 1 for on, 0 for off
private:
    long double x, y;
    long double width, height; // in pixels
    Texture buttonTex;
    Sprite buttonSprite;
    int isSmall = 0; // button dont become slower (when you tap it) if it slow
    long double scaleX, scaleY;
    long double textureW, textureH;
    int buttonMode = 1; // 1 for on, 0 for off
};

#endif

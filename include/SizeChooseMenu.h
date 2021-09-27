#ifndef SIZECHOOSEMENU_H
#define SIZECHOOSEMENU_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Button.h>
using namespace std;
using namespace sf;

class SizeChooseMenu
{
public:
    SizeChooseMenu();
    ~SizeChooseMenu();
    pair <int, int> display(RenderWindow *window);
private:
    Texture sizeChooseTexture;
    Sprite sizeChooseSprite;
    long double startX, startY, endX, endY, spaceForBorder;
    long double width, height, scale;
    int valueWidth = 1, valueHeight = 1;
    long double tileWidth;
    Button *sizeChooseClose;
    RectangleShape *sizeChooseRect;

    int mouseCheck();
};

#endif // SIZECHOOSEMENU_H

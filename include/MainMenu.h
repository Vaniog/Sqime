#ifndef MAINMENU_H
#define MAINMENU_H
#include <Button.h>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
class Button;
class MainMenu // some buttons in mainMenu
{
public:
    MainMenu();
    ~MainMenu();

    int drawAndCheckMenu(RenderWindow *window, float time);
private:
    int sizeX = 128 * 5, sizeY = 32 * 5;
    int startX = (VideoMode::getDesktopMode().width - sizeX) / 2;
    int startY = 200;
    int spaceY = 50;
    float scale = 3; // all configurable here

    float movingSpeed = 0.02, backStartX; // for "parallax" background

    Button *playButton = new Button(startX, startY, 0, 0, 128, 32, sizeX, sizeY, "images//PlayButton.png");
    Button *levelsButton = new Button(startX, startY + (sizeY + spaceY), 0, 0, 128, 32, sizeX, sizeY, "images//LevelsButton.png");
    Button *createButton = new Button(startX, startY + 2 * (sizeY + spaceY), 0, 0, 128, 32, sizeX, sizeY, "images//CreateButton.png");

    Texture backTex;
    int wB, hB; // width and height of background tile
    Sprite backSprite;

};

#endif // MAINMENU_H

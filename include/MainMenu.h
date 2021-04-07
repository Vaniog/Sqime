#ifndef MAINMENU_H
#define MAINMENU_H
#include <Button.h>
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

    Button *playButton;
    Button *levelsButton;
    Button *createButton;

    Texture backTex;
    int wB, hB; // width and height of background tile
    Sprite backSprite;

};

#endif // MAINMENU_H

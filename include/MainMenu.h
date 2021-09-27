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

    int drawAndCheckMenu(RenderWindow *window, long double time, bool off = 0);
private:
    int sizeX = 128 * 5, sizeY = 32 * 5;
    int startX = (VideoMode::getDesktopMode().width - sizeX) / 2;
    int startY = 200;
    int spaceY = 50;
    long double scale = 3; // all configurable here

    long double movingSpeed = 0.02, backStartX = 0; // for "parallax" background

    Button *exitButton;
    Button *mapButton;
    Button *createButton;

    Texture backTex;
    int wB, hB; // width and height of background tile
    Sprite backSprite;

};

#endif // MAINMENU_H

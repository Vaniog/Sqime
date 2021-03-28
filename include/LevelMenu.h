#ifndef LEVELMENU_H
#define LEVELMENU_H
#include <bits/stdc++.h>
#include <Button.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Button;

class LevelMenu // many buttons, which help you to choose levels
{
public:
    LevelMenu();
    int drawAndCheckMenu(sf::RenderWindow *window);

private:
    int startX, startY;
    int endX, endY;

    std::vector <Button*> levelsButtons;
    int spaceAroundButtons, spaceForButtons;
    int fullSpace; // spaceAround + spaceFor
    int buttonsInRow; // how many levelButtons will be in one row

    int levelsAmount;
    int spaceForText;
};


#endif // LEVELMENU_H

#include "LevelMenu.h"

LevelMenu::LevelMenu()
{
    int windowWidth, windowHeight;
    windowWidth = sf::VideoMode::getDesktopMode().width;
    windowHeight = sf::VideoMode::getDesktopMode().height;

    levelsAmount = 10; // const (just trust)
    spaceForButtons = 160; // const
    spaceAroundButtons = 40; // const
    spaceForText = spaceForButtons / 4;
    startX = 200;
    endX = windowWidth - startX;
    startY = 100;
    endY = windowHeight - startY;

    fullSpace = (spaceForButtons + 2 * spaceAroundButtons);
    buttonsInRow = (endX - startX) / fullSpace;
    int i;

    for (i = 0; i < levelsAmount; i++)
    {
        Button *button = new Button(startX + i % buttonsInRow * fullSpace , startY + fullSpace * (i / buttonsInRow), 0, 0, 16, 16, spaceForButtons, spaceForButtons, "images//button.png");
        levelsButtons.push_back(button);
    }
}

int LevelMenu::drawAndCheckMenu(sf::RenderWindow *window)
{
    int i;

    sf::Font font;
    font.loadFromFile("images//mainFont.ttf");
    sf::Text levelText("", font, spaceForButtons - spaceForText * 2);
    levelText.setScale(1, 1);
    levelText.setColor(sf::Color(0, 0, 0, 255));

    for (i = 0; i < levelsAmount; i++)
    {
        int buttonCondition = levelsButtons[i]->buttonDisplayAndCheck(window, -1, -1);
        if (buttonCondition == 1)
            return i + 1;

        char s[3]; // its algorithm work only with two-digit numbers, because it simple
        s[0] = '0' + (i + 1) / 10;
        s[1] = '0' + (i + 1) % 10;
        s[2] = '\0';
        levelText.setString(s);
        if (buttonCondition != 3)
        {
            levelText.setPosition(spaceForText + startX + i % buttonsInRow * fullSpace, spaceForText + startY + fullSpace * (i / buttonsInRow) - spaceForButtons / 16);
            levelText.setScale(1, 1);
        }
        else
        {
            levelText.setPosition((float)spaceForText + ((float)spaceForButtons / 40) + startX + i % buttonsInRow * fullSpace, (float)spaceForText + ((float)spaceForButtons / 40) + startY + fullSpace * (i / buttonsInRow) - spaceForButtons / 16);
            levelText.setScale(0.95, 0.95);
        }
        window->draw(levelText);
    }
    return 0;
}

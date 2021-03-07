#include "LevelMenu.h"

LevelMenu::LevelMenu()
{
    int windowWidth, windowHeight;
    windowWidth = VideoMode::getDesktopMode().width;
    windowHeight = VideoMode::getDesktopMode().height;

    levelsAmount = 10; // const (just trust)
    spaceForButtons = 150; // const
    spaceAroundButtons = 40; // const
    spaceForText = spaceForButtons / 5;
    startX = 200;
    endX = windowWidth - startX;
    startY = 100;
    endY = windowHeight - startY;

    fullSpace = (spaceForButtons + 2 * spaceAroundButtons);
    buttonsInRow = (endX - startX) / fullSpace;
    int i;

    for (i = 0; i < levelsAmount; i++)
    {
        Button *button = new Button(startX + i % buttonsInRow * fullSpace , startY + fullSpace * (i / buttonsInRow), 0, 0, 32, 32, spaceForButtons, spaceForButtons, "images\\button.png");
        levelsButtons.push_back(button);
    }
}

int LevelMenu::drawAndCheckMenu(RenderWindow *window)
{
    int i;

    Font font;
    font.loadFromFile("images\\mainFont.ttf");
    Text levelText("", font, spaceForButtons - spaceForText * 2);
    levelText.setColor(Color(0, 0, 0));

    for (i = 0; i < levelsAmount; i++)
    {
        if (levelsButtons[i]->buttonDisplayAndCheck(window, -1, -1) == 1)
            return i + 1;

        char s[3]; // its algorithm work only with two-digit numbers, because it simple
        s[0] = '0' + (i + 1) / 10;
        s[1] = '0' + (i + 1) % 10;
        s[2] = '\0';
        levelText.setString(s);
        levelText.setPosition(spaceForText + startX + i % buttonsInRow * fullSpace, spaceForText + startY + fullSpace * (i / buttonsInRow));
        window->draw(levelText);
    }
    return 0;
}

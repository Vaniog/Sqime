#include "MainMenu.h"

MainMenu::MainMenu()
{
    playButton = new Button(startX, startY, 0, 0, 128, 32, sizeX, sizeY, "images//PlayButton.png");
    levelsButton = new Button(startX, startY + (sizeY + spaceY), 0, 0, 128, 32, sizeX, sizeY, "images//LevelsButton.png");
    createButton = new Button(startX, startY + 2 * (sizeY + spaceY), 0, 0, 128, 32, sizeX, sizeY, "images//CreateButton.png");
    Image backImg;
    backImg.loadFromFile("images//mainMenuBack.png");
    wB = backImg.getSize().x, hB = backImg.getSize().y;
    backTex.loadFromImage(backImg);
    backSprite.setTexture(backTex);
    backSprite.setScale(scale, scale);
    backStartX = -wB * scale;
}

MainMenu::~MainMenu()
{
    delete levelsButton;
    delete playButton;
    delete createButton;
}

int MainMenu::drawAndCheckMenu(RenderWindow *window, float time)
{
    int i, k;
    for (i = 0; i <= (VideoMode::getDesktopMode().width - backStartX) / wB / scale; i++)
    {
        for (k = 0; k <= VideoMode::getDesktopMode().height / hB / scale; k++)
            backSprite.setPosition(i * wB * scale + backStartX, k * hB * scale), window->draw(backSprite);
    }

    backStartX += movingSpeed * time;
    if (backStartX >= 0)
        backStartX = -wB * scale;
    if (playButton->buttonDisplayAndCheck(window, -1, -1) == 1) // if it will be 2, it mean you tapped by right button
        return 1;
    if (levelsButton->buttonDisplayAndCheck(window, -1, -1) == 1)
        return 2;
    if (createButton->buttonDisplayAndCheck(window, -1, -1) == 1)
        return 3;
    return 0;
}

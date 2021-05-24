#include "MainMenu.h"

MainMenu::MainMenu()
{
    float buttonsScale = 5;
    int mBSizeX = 68, mBSizeY = 44;
    int cBSizeX = 84, cBSizeY = 48;
    int eBSizeX = 68, eBSizeY = 44;

    int mBStartX = 2, mBStartY = 3;
    int cBStartX = 4, cBStartY = 6;
    int eBStartX = 6, eBStartY = 2;

    int tileAmountX = 10, tileAmountY = 10;
    int stepX = VideoMode::getDesktopMode().width / tileAmountX, stepY = VideoMode::getDesktopMode().height / tileAmountX;

    exitButton = new Button(stepX * eBStartX, stepY * eBStartY, 0, 0, eBSizeX, eBSizeY, eBSizeX * buttonsScale, eBSizeY * buttonsScale, "images//ExitButton.png");
    mapButton = new Button(stepX * mBStartX, stepY * mBStartY, 0, 0, mBSizeX, mBSizeY, mBSizeX * buttonsScale, mBSizeY * buttonsScale, "images//MapButton.png");
    createButton = new Button(stepX * cBStartX, stepY * cBStartY, 0, 0, cBSizeX, cBSizeY, cBSizeX * buttonsScale, cBSizeY * buttonsScale, "images//CreateButton.png");
    Image backImg;
    backImg.loadFromFile("images//mainMenuBack.png");
    wB = backImg.getSize().x, hB = backImg.getSize().y;
    backTex.loadFromImage(backImg);
    int i = 10; while(i){i--;};
    backSprite.setTexture(backTex);
    backSprite.setScale(buttonsScale, buttonsScale);
    scale = buttonsScale;
}

MainMenu::~MainMenu()
{
    delete mapButton;
    delete exitButton;
    delete createButton;
}

int MainMenu::drawAndCheckMenu(RenderWindow *window, float time, bool off)
{
    if (off)
    {
        exitButton->buttonModeSet(0);
        mapButton->buttonModeSet(0);
        createButton->buttonModeSet(0);
    }
    else
    {
        exitButton->buttonModeSet(1);
        mapButton->buttonModeSet(1);
        createButton->buttonModeSet(1);
    }

    int i, k;
    int returnValue = 0;
    for (i = 0; i <= (VideoMode::getDesktopMode().width - backStartX) / wB / scale; i++)
    {
        for (k = 0; k <= VideoMode::getDesktopMode().height / hB / scale; k++)
            backSprite.setPosition(i * wB * scale, k * hB * scale), window->draw(backSprite);
    }

    if (exitButton->buttonDisplayAndCheck(window, -1, -1) == 1) // if it will be 2, it mean you tapped by right button
        returnValue = 1;
    if (mapButton->buttonDisplayAndCheck(window, -1, -1) == 1)
        returnValue = 2;
    if (createButton->buttonDisplayAndCheck(window, -1, -1) == 1)
        returnValue = 3;
    return returnValue;
}


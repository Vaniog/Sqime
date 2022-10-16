#include "SizeChooseMenu.h"

SizeChooseMenu::SizeChooseMenu()
{
    Image sizeChooseImage;
    sizeChooseImage.loadFromFile("images//SizeChooseMenu.png");
    height = VideoMode::getDesktopMode().height / 2;
    width = height;
    scale = width / sizeChooseImage.getSize().x;

    spaceForBorder = width / 8;

    startX = VideoMode::getDesktopMode().width / 2 -  width / 2 + spaceForBorder;
    startY = VideoMode::getDesktopMode().height / 2 -  height / 2 + spaceForBorder;
    endX = startX + width - spaceForBorder * 2;
    endY = startY + height - spaceForBorder * 2;
    sizeChooseClose = new Button(endX + spaceForBorder - scale * 4, startY - spaceForBorder - scale * 4, 0, 0, 9, 9, 9 * scale, 9 * scale, "images//CloseSizeChoose.png");

    sizeChooseTexture.loadFromImage(sizeChooseImage);
    sizeChooseSprite.setTexture(sizeChooseTexture);
    sizeChooseSprite.setPosition(startX - spaceForBorder, startY - spaceForBorder);
    sizeChooseSprite.setScale(scale, scale);

    tileWidth = (endX - startX - scale) / 5;
    sizeChooseRect = new RectangleShape(Vector2f(tileWidth - scale, tileWidth - scale));
    sizeChooseRect->setOutlineColor(Color(162, 191, 207));
    sizeChooseRect->setOutlineThickness(scale + 1);
    sizeChooseRect->setFillColor(Color(0, 0, 0, 0));
}

SizeChooseMenu::~SizeChooseMenu()
{
    delete sizeChooseClose;
    delete sizeChooseRect;
}

pair <int, int> SizeChooseMenu::display(RenderWindow *window)
{
    pair <int, int> sizes = {0, 0};
    int mouseCheckReturn = mouseCheck(window);
    sizeChooseRect->setSize(Vector2f(tileWidth * valueWidth - scale, tileWidth * valueHeight - scale));
    sizeChooseRect->setPosition(startX + scale, startY + scale);
    window->draw(sizeChooseSprite);
    window->draw(*sizeChooseRect);
    if (sizeChooseClose->buttonDisplayAndCheck(window, -1, -1) == 1)
    {
        sizes.first = -1;
        return sizes;
    }
    else if(mouseCheckReturn == 1)
    {
        sizes.first = valueWidth;
        sizes.second = valueHeight;
        return sizes;
    }
    return sizes;
}


int SizeChooseMenu::mouseCheck(sf::RenderWindow *window)
{
    long double mouseX = Mouse::getPosition(*window).x;
    long double mouseY = Mouse::getPosition(*window).y;
    if (mouseX >= startX && mouseX <= endX - scale && mouseY >= startY && mouseY <= endY - scale)
    {
        valueWidth = (mouseX - startX) / tileWidth + 1;
        valueHeight = (mouseY - startY) / tileWidth + 1;
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            while(Mouse::isButtonPressed(Mouse::Left)){};
            return 1;
        }
    }
    return 0;
}

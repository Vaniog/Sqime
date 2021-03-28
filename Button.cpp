#include "Button.h"

Button::Button(int xIn, int yIn, int startDownloadingFromX, int startDownloadingFromY, int textureWIn, int textureHIn, float widthIn, float heightIn, std::string buttonTexPlace)
{
    x = xIn, y = yIn;
    width = widthIn, height = heightIn;
    sf::Image buttonImage;
    buttonImage.loadFromFile(buttonTexPlace);
    buttonTex.loadFromImage(buttonImage);
    buttonSprite.setTexture(buttonTex);
    if (textureWIn == -1)
    {
        textureWIn = buttonImage.getSize().x;
        textureHIn = buttonImage.getSize().y;
    }
    buttonSprite.setTextureRect(sf::IntRect(startDownloadingFromX, startDownloadingFromY, textureWIn, textureHIn));
    buttonSprite.setPosition(x, y);
    textureW = textureWIn;
    textureH = textureHIn;
    scaleX = width / textureW;
    scaleY = height / textureH;
    buttonSprite.setScale(scaleX, scaleY);
}

int Button::buttonDisplayAndCheck(sf::RenderWindow *window, int xIn, int yIn)
{
    if (xIn != -1)
    {
        x = xIn, y = yIn;
    }

    bool isMouseOnButton = 0;
    int mouseX = sf::Mouse::getPosition().x;
    int mouseY = sf::Mouse::getPosition().y;
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (buttonMode)
            {
                while(sf::Mouse::isButtonPressed(sf::Mouse::Left));
                return 1;
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                if (buttonMode)
                {
                    while(sf::Mouse::isButtonPressed(sf::Mouse::Right));
                    return 2;
                }
            }
            else
                if (isSmall == 0)
                {
                    buttonSprite.setScale(scaleX * 0.95, scaleY * 0.95), isSmall = 1;
                }
        isMouseOnButton = 1;
    }
    else
        if (isSmall == 1)
        {
            buttonSprite.setScale(scaleX, scaleY), isSmall = 0;
        }

    if (isSmall == 1)
        buttonSprite.setPosition(x + (int)((textureW * scaleX - textureW * scaleX * 0.95) / 2), y + (int)((textureH * scaleY - textureH * scaleY * 0.95) / 2));
    else
        buttonSprite.setPosition(x, y);
    (*window).draw(buttonSprite);

    if (isMouseOnButton == 1)
        return 3;
    return 0;
}

void Button::buttonModeSet(int mode)
{
    buttonMode = mode;
}

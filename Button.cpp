#include "Button.h"

Button::Button(int xIn, int yIn, int startDownloadingFromX, int startDownloadingFromY, int textureWIn, int textureHIn, float widthIn, float heightIn, string buttonTexPlace)
{
    x = xIn, y = yIn;
    width = widthIn, height = heightIn;
    Image buttonImage;
    buttonImage.loadFromFile(buttonTexPlace);
    buttonTex.loadFromImage(buttonImage);
    buttonSprite.setTexture(buttonTex);
    if (textureWIn == -1)
    {
        textureWIn = buttonImage.getSize().x;
        textureHIn = buttonImage.getSize().y;
    }
    buttonSprite.setTextureRect(IntRect(startDownloadingFromX, startDownloadingFromY, textureWIn, textureHIn));
    buttonSprite.setPosition(x, y);
    textureW = textureWIn;
    textureH = textureHIn;
    scaleX = width / textureW;
    scaleY = height / textureH;
    buttonSprite.setScale(scaleX, scaleY);
}

int Button::buttonDisplayAndCheck(RenderWindow *window, int xIn, int yIn)
{
    if (xIn != -1)
    {
        x = xIn, y = yIn;
    }

    int mouseX = Mouse::getPosition().x;
    int mouseY = Mouse::getPosition().y;
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
    {
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            while(Mouse::isButtonPressed(Mouse::Left));
            return 1;
        }
        else if (Mouse::isButtonPressed(Mouse::Right))
            {
                while(Mouse::isButtonPressed(Mouse::Right));
                return 2;
            }
            else
                if (isSmall == 0)
                {
                    buttonSprite.setScale(scaleX * 0.95, scaleY * 0.95), isSmall = 1;
                }
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

    return 0;
}

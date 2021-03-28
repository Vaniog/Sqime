#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>


class Slider // Slider help you input inf with slider
{
public:
    Slider(float textX, float textY, int textSize, int lineStartXIn, int lineStartYIn, int lineEndXIn, int lineEndYIn, int minValueIn, int maxValueIn, int sliderDiametrIn, float stepIn, std::string sliderPlace)
    {
        lineStartX = lineStartXIn, lineStartY = lineStartYIn;
        lineEndX = lineEndXIn, lineEndY = lineEndYIn;
        minValue = minValueIn, maxValue = maxValueIn;
        sliderDiametr = sliderDiametrIn;
        step = stepIn;
        lineThick = sliderDiametr / 16;

        sf::Image sImage;
        sImage.loadFromFile(sliderPlace);
        scale = sliderDiametr / sImage.getSize().x;
        sliderTexture.loadFromImage(sImage);
        sliderSprite.setTexture(sliderTexture);
        sliderSprite.setScale(scale, scale);

        font.loadFromFile("images//mainFont.ttf");
        valueText = new sf::Text("", font, textSize);
        valueText->setFillColor(sf::Color(0, 0, 0));
        valueText->setPosition(textX, textY);

        if (lineStartY == lineEndY)
        {
            rotation = 1;
            line = new sf::RectangleShape(sf::Vector2f(lineEndX - lineStartX, 0));
            pixelStep = (lineEndX - lineStartX) / (maxValue - minValue);
        }
        else
        {
            rotation = 2;
            line = new sf::RectangleShape(sf::Vector2f(0, lineEndY - lineStartY));
            pixelStep = (lineEndY - lineStartY) / (maxValue - minValue);
        }

        line->setPosition(lineStartX, lineStartY);
        line->setOutlineThickness(lineThick);
        line->setOutlineColor(sf::Color(32, 47, 57));

        mainValue = minValue;
        sliderCurX = lineStartX;
        sliderCurY = lineStartY;
        //value
    }


    ~Slider()
    {
        delete valueText;
        delete line;
    }

    float drawSliderGetValue(sf::RenderWindow *window)
    {
        switch (rotation)
        {
        case 1:
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int mouseX = sf::Mouse::getPosition().x;
                int mouseY = sf::Mouse::getPosition().y;
                if ((mouseX <= lineEndX && mouseX >= lineStartX) && (remTouch == 1 || (mouseY <= lineStartY + sliderDiametr / 2 && mouseY >= lineStartY - sliderDiametr / 2)))
                {
                    sliderCurX = mouseX, remTouch = 1;

                    if ((sliderCurX - lineStartX) - (int)((sliderCurX - lineStartX) / pixelStep) < pixelStep / 2)
                        mainValue = (int)(((sliderCurX - lineStartX) / pixelStep) / step) * step + minValue;
                    else
                        mainValue = (int)(((sliderCurX - lineStartX) / pixelStep) / step) * step + step + minValue;
                }
            }
            else
                sliderCurX = lineStartX + (mainValue - minValue) * pixelStep, remTouch = 0;

            sliderSprite.setPosition(sliderCurX - sliderDiametr / 2, sliderCurY - sliderDiametr / 2);
            break;
        }
        case 2:
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int mouseX = sf::Mouse::getPosition().x;
                int mouseY = sf::Mouse::getPosition().y;
                if ((mouseY <= lineEndY && mouseY >= lineStartY) && (remTouch == 1 || (mouseX <= lineStartX + sliderDiametr / 2 && mouseX >= lineStartX - sliderDiametr / 2)))
                {
                    sliderCurY = mouseY, remTouch = 1;

                    if ((sliderCurY - lineStartY) - (int)((sliderCurY - lineStartY) / pixelStep) * pixelStep < pixelStep / 2)
                        mainValue = (int)(((sliderCurY - lineStartY) / pixelStep) / step) * step + minValue;
                    else
                        mainValue = (int)(((sliderCurY - lineStartY) / pixelStep) / step) * step + step + minValue;
                }
            }
            else
                sliderCurY = lineStartY + (mainValue - minValue) * pixelStep, remTouch = 0;

            sliderSprite.setPosition(sliderCurX - sliderDiametr / 2, sliderCurY - sliderDiametr / 2);
            break;
        }
        }



        std::string s = std::to_string((int)mainValue);
        if (step < 1)
            s += "." + std::to_string((int)((mainValue + 0.001) * 10) % 10);
        valueText->setString(s);
        window->draw(*valueText);
        window->draw(*line);
        window->draw(sliderSprite);
        return mainValue;
    }

private:
    float lineStartX, lineStartY, lineEndX, lineEndY, maxValue, minValue, step, pixelStep; // step is value step, pixelStep how much you need to go to pass step
    int sliderCurX, sliderCurY;
    int sliderDiametr; // in pixels
    int rotation; // 1 if slide horizontal, 2 if slider vertical
    bool remTouch = 0; // remember touch, it help you move slider after tap on it
    float scale;
    int lineThick;
    float mainValue;
    sf::Texture sliderTexture;
    sf::Sprite sliderSprite;
    sf::Font font;
    sf::Text *valueText;
    sf::RectangleShape *line;
};


class Button // its button
{
public:
    Button(int xIn, int yIn, int startDownloadingFromX, int startDownloadingFromY, int textureW, int textureH, float widthIn, float heightIn, std::string buttonTexPlace);

    int buttonDisplayAndCheck(sf::RenderWindow *window, int xIn, int yIn);
    void buttonModeSet(int mode); // 1 for on, 0 for off
private:
    float x, y;
    float width, height; // in pixels
    sf::Texture buttonTex;
    sf::Sprite buttonSprite;
    int isSmall = 0; // button dont become slower (when you tap it) if it slow
    float scaleX, scaleY;
    float textureW, textureH;
    int buttonMode = 1; // 1 for on, 0 for off
};

#endif

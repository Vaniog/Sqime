#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;


class Slider // Slider help you input inf with slider
{
public:
    Slider(int textX, int textY, int textSize, int lineStartXIn, int lineStartYIn, int lineEndXIn, int lineEndYIn, int minValueIn, int maxValueIn, int sliderDiametrIn, float stepIn, string sliderPlace)
    {
        lineStartX = lineStartXIn, lineStartY = lineStartYIn;
        lineEndX = lineEndXIn, lineEndY = lineEndYIn;
        minValue = minValueIn, maxValue = maxValueIn;
        sliderDiametr = sliderDiametrIn;
        step = stepIn;

        Image sImage;
        sImage.loadFromFile(sliderPlace);
        scale = sliderDiametr / sImage.getSize().x;
        sliderTexture.loadFromImage(sImage);
        sliderSprite.setTexture(sliderTexture);
        sliderSprite.setScale(scale, scale);

        font.loadFromFile("images\\mainFont.ttf");
        valueText = new Text("", font, textSize);
        valueText->setFillColor(Color(0, 0, 0));
        valueText->setPosition(textX, textY);

        if (lineStartY == lineEndY)
        {
            rotation = 1;
            line = new RectangleShape(sf::Vector2f(lineEndX - lineStartX, lineThick));
            pixelStep = (lineEndX - lineStartX) / (maxValue - minValue);
        }
        else
        {
            rotation = 2;
            line = new RectangleShape(sf::Vector2f(lineThick, lineEndY - lineStartY));
            pixelStep = (lineEndY - lineStartY) / (maxValue - minValue);
        }

        line->setFillColor(Color(32, 47, 57));
        line->setPosition(lineStartX, lineStartY);

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

    float drawSliderGetValue(RenderWindow *window)
    {
        switch (rotation)
        {
        case 1:
        {
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                int mouseX = Mouse::getPosition().x;
                int mouseY = Mouse::getPosition().y;
                if ((mouseX <= lineEndX && mouseX >= lineStartX) && (remTouch == 1 || (mouseY <= lineStartY + sliderDiametr / 2 && mouseY >= lineStartY - sliderDiametr / 2)))
                {
                    sliderCurX = mouseX, remTouch = 1;
                }

                if ((sliderCurX - lineStartX) - (int)((sliderCurX - lineStartX) / pixelStep) * pixelStep < pixelStep / 2)
                    mainValue = (int)((sliderCurX - lineStartX) / pixelStep) + minValue;
                else
                    mainValue = (int)((sliderCurX - lineStartX) / pixelStep) + step + minValue;
            }
            else
                sliderCurX = lineStartX + (mainValue - minValue) * pixelStep, remTouch = 0;

            sliderSprite.setPosition(sliderCurX - sliderDiametr / 2, sliderCurY - sliderDiametr / 2 + lineThick / 2);
            break;
        }
        case 2:
        {
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                int mouseX = Mouse::getPosition().x;
                int mouseY = Mouse::getPosition().y;
                if ((mouseY <= lineEndY && mouseY >= lineStartY) && (remTouch == 1 || (mouseX <= lineStartX + sliderDiametr / 2 && mouseX >= lineStartX - sliderDiametr / 2)))
                {
                    sliderCurY = mouseY, remTouch = 1;
                }

                if ((sliderCurY - lineStartY) - (int)((sliderCurY - lineStartY) / pixelStep) * pixelStep < pixelStep / 2)
                    mainValue = (int)((sliderCurY - lineStartY) / pixelStep) + minValue;
                else
                    mainValue = (int)((sliderCurY - lineStartY) / pixelStep) + step + minValue;
            }
            else
                sliderCurY = lineStartY + (mainValue - minValue) * pixelStep, remTouch = 0;

            sliderSprite.setPosition(line->getPosition().x + line->getSize().x / 2 - sliderDiametr / 2, sliderCurY - sliderDiametr / 2);
            break;
        }
        }



        string s = to_string((int)mainValue);
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
    int lineThick = 6;
    float mainValue;
    Texture sliderTexture;
    Sprite sliderSprite;
    Font font;
    Text *valueText;
    RectangleShape *line;
};


class Button // its button
{
public:
    Button(int xIn, int yIn, int startDownloadingFromX, int startDownloadingFromY, int textureW, int textureH, float widthIn, float heightIn, string buttonTexPlace);

    int buttonDisplayAndCheck(RenderWindow *window, int xIn, int yIn);
private:
    float x, y;
    float width, height; // in pixels
    Texture buttonTex;
    Sprite buttonSprite;
    int isSmall = 0; // button dont become slower (when you tap it) if it slow
    float scaleX, scaleY;
    float textureW, textureH;
};

#endif

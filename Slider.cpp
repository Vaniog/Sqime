#include "Slider.h"

Slider::Slider(long double textX, long double textY, int textSize, int lineStartXIn, int lineStartYIn, int lineEndXIn, int lineEndYIn, int minValueIn, int maxValueIn, int sliderDiametrIn, long double stepIn, string sliderPlace)
{
    lineStartX = lineStartXIn, lineStartY = lineStartYIn;
    lineEndX = lineEndXIn, lineEndY = lineEndYIn;
    minValue = minValueIn, maxValue = maxValueIn;
    sliderDiametr = sliderDiametrIn;
    step = stepIn;
    lineThick = sliderDiametr / 16;

    Image sImage;
    sImage.loadFromFile(sliderPlace);
    scale = sliderDiametr / sImage.getSize().x;
    sliderTexture.loadFromImage(sImage);
    sliderSprite.setTexture(sliderTexture);
    sliderSprite.setScale(scale, scale);

    font.loadFromFile("images//mainFont.ttf");
    valueText = new Text("", font, textSize);
    valueText->setFillColor(Color(0, 0, 0));
    valueText->setPosition(textX, textY);

    if (lineStartY == lineEndY)
    {
        rotation = 1;
        line = new RectangleShape(sf::Vector2f(lineEndX - lineStartX, 0));
        pixelStep = (lineEndX - lineStartX) / (maxValue - minValue);
    }
    else
    {
        rotation = 2;
        line = new RectangleShape(sf::Vector2f(0, lineEndY - lineStartY));
        pixelStep = (lineEndY - lineStartY) / (maxValue - minValue);
    }

    line->setPosition(lineStartX, lineStartY);
    line->setOutlineThickness(lineThick);
    line->setOutlineColor(Color(32, 47, 57));

    mainValue = minValue;
    sliderCurX = lineStartX;
    sliderCurY = lineStartY;
    //value
}


Slider::~Slider()
{
    delete valueText;
    delete line;
}

long double Slider::drawSliderGetValue(RenderWindow *window)
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
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            int mouseX = Mouse::getPosition().x;
            int mouseY = Mouse::getPosition().y;
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



    string s = to_string((int)mainValue);
    if (step < 1)
        s += "." + to_string((int)((mainValue + 0.001) * 10) % 10);
    valueText->setString(s);
    window->draw(*valueText);
    window->draw(*line);
    window->draw(sliderSprite);
    return mainValue;
}

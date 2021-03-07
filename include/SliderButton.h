#ifndef SLIDERBUTTON_H
#define SLIDERBUTTON_H

#include <SFML/Graphics.hpp>

class SliderButton
{
    public:
        SliderButton(int startX, int startY, string variable)
        {
            UITex.loadFromFile("images\\Slider.png");
            sliderSprite.
        }

    private:
        Texture UITex;
        Sprite UISprite;
        RectangleShape slider(5, 10);
};

#endif // SLIDERBUTTON_H

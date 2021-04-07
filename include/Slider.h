#ifndef SLIDER_H
#define SLIDER_H
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Slider // Slider help you input inf with slider
{
public:
    Slider(float textX, float textY, int textSize, int lineStartXIn, int lineStartYIn, int lineEndXIn, int lineEndYIn, int minValueIn, int maxValueIn, int sliderDiametrIn, float stepIn, string sliderPlace);
    ~Slider();
    float drawSliderGetValue(RenderWindow *window);

private:
    float lineStartX, lineStartY, lineEndX, lineEndY, maxValue, minValue, step, pixelStep; // step is value step, pixelStep how much you need to go to pass step
    int sliderCurX, sliderCurY;
    int sliderDiametr; // in pixels
    int rotation; // 1 if slide horizontal, 2 if slider vertical
    bool remTouch = 0; // remember touch, it help you move slider after tap on it
    float scale;
    int lineThick;
    float mainValue;
    Texture sliderTexture;
    Sprite sliderSprite;
    Font font;
    Text *valueText;
    RectangleShape *line;
};
#endif // SLIDER_H

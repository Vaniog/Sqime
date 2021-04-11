#ifndef DYNAMICBUTTONCREATINGUI_H
#define DYNAMICBUTTONCREATINGUI_H
#include "Button.h"
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

struct ButtonsInf
{
public:
    ButtonsInf(int width, int height, float scaleIn)
    {
        scale = scaleIn;
        sizes[0] = width;
        sizes[1] = height;
        rect.setSize(Vector2f((float)width * scale, (float)height * scale));
        rect.setFillColor(Color(255, 255, 255, 70));
    }
    int value[2] = {0, 0}; // first - real, second - what mouse touch
    int addButton(int x, int y)
    {
        vector <int> a = {x, y};
        coords.push_back(a);
        amount++;
    }
    int checkButtonDrawRect(int mouseX, int mouseY, int isPressed, RenderWindow *window, int startX, int startY)
    {
        int i;
        value[1] = value[0];
        for (i = 0; i < amount; i++)
            if (mouseX + 1 >= coords[i][0] && mouseX - 1 <= coords[i][0] + sizes[0] && mouseY + 1 >= coords[i][1] && mouseY - 1<= coords[i][1] + sizes[1])
            {
                if (isPressed)
                    value[0] = i, value[1] = i;
                else
                    value[1] = i;
            }
        rect.setPosition(startX + scale * coords[value[1]][0], startY + scale * coords[value[1]][1]);
        window->draw(rect);
    }
private:
    vector <vector <int>> coords;
    int sizes[2];
    int amount = 0;
    float scale;
    RectangleShape rect;
};

class DynamicButtonCreatingUI
{
public:
    DynamicButtonCreatingUI(int startXIn, int startYIn, int width);
    int getValue(char valueName);
    ~DynamicButtonCreatingUI();
    int draw(RenderWindow *window);
private:
    int startX, startY;
    float scale;
    Texture uiTexture;
    Sprite uiSprite;

    ButtonsInf *buttonMode;
    ButtonsInf *buttonOff;
    ButtonsInf *buttonOn;
    ButtonsInf *buttonLength;
    ButtonsInf *buttonDir;
    Button *okButton;
};

#endif // DYNAMICBUTTONCREATINGUI_H

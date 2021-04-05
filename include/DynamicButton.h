#ifndef DYNAMICBUTTON_H
#define DYNAMICBUTTON_H
#include "SFML/Graphics.hpp"
#include "DynamicObject.h"
#include "AllHitboxInf.h"
#include "MyMap.h"
using namespace std;
using namespace sf;

class DynamicButton : public DynamicObject
{
    public:
        DynamicButton(int curXIn, int curYIn, int length, int direction, MyMap *mapIn, AllHitboxInf *AHIIn, string DynamicButtonPlace);
        ~DynamicButton();

        char type();
        pair <float, float> coordinates();
        pair <float, float> sizes();

        int tryToMove(float distance, int direction, int mode);
        int tryToSquezze(float distance, int direction, int mode);
        void drawObject(float &time);
        void displayObject(RenderWindow *window);

        int getNumber();
        void setNumber(int n);
    private:
        AllHitboxInf *AHI;
        float width, height;
        int startMapX, startMapY;
        float startX, startY;
        float curX, curY;
        float endX, endY;
        float tilesize, scale;
        int number;
        int dbDirection;
        Texture dbTexture; // Dynamic Button
        Sprite dbSprite;
};

#endif // DYNAMICBUTTON_H

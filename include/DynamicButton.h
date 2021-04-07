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
        DynamicButton(int curXIn, int curYIn, int length, int direction, int controlledObjectIn, int buttonControlMode, int onObjMode, int offObjMode,MyMap *mapIn, AllHitboxInf *AHIIn, string DynamicButtonPlace);
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
        Texture dbTexture; // db - Dynamic Button
        Sprite dbSprite;

        int buttonControlMode; // 0: pressed - on, unpressed - off, // 1: switching on and off by pressing
        int controlledObject; // number of control object in AHI
        int modesToSend[2]; // first for on, second for off
        // modes: 0 - both direction, 1 - plus direction, 2 - main direction, 3 - stop moving
        int modeToSendNow = 1; // help for nice code
        void buttonBecome(int location); // 0 for up, 1 for down
};

#endif // DYNAMICBUTTON_H

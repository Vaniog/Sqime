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
        pair <long double, long double> coordinates();
        pair <long double, long double> sizes();

        int tryToMove(long double distance, int direction, int mode);
        int tryToSquezze(long double distance, int direction, int mode);
        void drawObject(long double &time);
        void displayObject(RenderWindow *window);

        int getNumber();
        void setNumber(int n);
    private:
        AllHitboxInf *AHI;
        long double width, height;
        int startMapX, startMapY;
        long double startX, startY;
        long double curX, curY;
        long double endX, endY;
        long double tilesize, scale;
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

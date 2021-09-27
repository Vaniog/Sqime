#ifndef PLATFORM_H
#define PLATFORM_H
#include <SFML/Graphics.hpp>
#include <DynamicObject.h>
#include <MyMap.h>
using namespace std;
using namespace sf;
class MyMap;
class AllHitboxInf;
class Platform: public DynamicObject
{
public:
    long double curX, curY;
    long double width, height;
    long double weight;
    int number;

    Platform(long double startXIn, long double startYIn, long double endXIn, long double endYIn, long double widthIn, long double heightIn, long double speed, string texPlace, MyMap *MapIn, AllHitboxInf *AHIIn);
    void drawObject(long double &time);
    void displayObject(RenderWindow *window);
    void sendMessage(long double message);
    pair <long double, long double> coordinates();
    pair <long double, long double> sizes();
    int getNumber();
    void setNumber(int n);
private:
    long double startX, startY;
    long double endX, endY;
    long double startXMain, startYMain;
    long double endXMain, endYMain;
    int dir;
    long double scale;
    long double tilesize;
    long double platformSpeed;
    long double mapStartX, mapStartY;
    Texture platformTexture;
    Sprite platformSprite;
    AllHitboxInf *AHI;

    void movePlatform(long double time, AllHitboxInf *AHI);
    int tryToMove(long double distance, int direction, int mode); // return -1;
    int tryToSquezze(long double distance, int direction, int mode);
    char type();
};

#endif // PLATFORM_H

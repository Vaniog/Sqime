#ifndef BOX_H
#define BOX_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <DynamicObject.h>
#include <AllHitboxInf.h>
#include <MyMap.h>
#define MaxMapW 50
#define MaxMapH 50
using namespace std;
using namespace sf;

class Box: public DynamicObject
{
public:
    long double curX , curY;
    int number = 0;
    Box(string boxTexturePlace, int startX, int startY, int w, int h, MyMap *MapIn, AllHitboxInf *AHIIn);
    void drawObject(long double &time);
    void displayObject(RenderWindow *window);
    int tryToMove(long double distance, int direction, int mode);
    int getNumber();
    void setNumber(int n);
    pair <long double, long double> coordinates();
    pair <long double, long double> sizes();

private:
    long double scale;
    long double weight;
    long double freeFallAcc, fallSpeed, maxFallSpeed;
    long double realY, lastCurY;

    long double startX, startY;
    long double mapWidth, mapHeight;
    long double height, width;

    AllHitboxInf *AHI;
    int windowWidth, windowHeight;
    bool onGround = false;
    int physics[MaxMapW][MaxMapH], tilesize, ptilesize;
    Sprite boxSprite;
    Texture boxTexture;

    void Update(long double &time);

    int isTouching(long double newX, long double newY, int direction);
    int tryToSquezze(long double distance, int direction, int mode);
    char type();
};
#endif // BOX_H

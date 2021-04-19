#ifndef BOX_H
#define BOX_H


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <DynamicObject.h>
#include <AllHitboxInf.h>
#include <MyMap.h>
#define MaxMapW 50
#define MaxMapH 50

class Box: public DynamicObject
{
public:
    float curX , curY;
    int number = 0;
    Box(string boxTexturePlace, int startX, int startY, int w, int h, MyMap *MapIn, AllHitboxInf *AHIIn);
    void drawObject(float &time);
    void displayObject(RenderWindow *window);
    int tryToMove(float distance, int direction, int mode);
    int getNumber();
    void setNumber(int n);
    pair <float, float> coordinates();
    pair <float, float> sizes();

private:
    float scale;
    float weight;
    float freeFallAcc, fallSpeed, maxFallSpeed;
    float realY, lastCurY;

    float startX, startY;
    float mapWidth, mapHeight;
    float height, width;

    AllHitboxInf *AHI;
    int windowWidth, windowHeight;
    bool onGround = false;
    int physics[MaxMapW][MaxMapH], tilesize, ptilesize;
    Sprite boxSprite;
    Texture boxTexture;

    void Update(float &time);

    int isTouching(float newX, float newY, int direction);
    int tryToSquezze(float distance, int direction, int mode);
    char type();
};
#endif // BOX_H

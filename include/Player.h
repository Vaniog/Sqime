#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <DynamicObject.h>
#include <AllHitboxInf.h>
#include <MyMap.h>
#include <floatComparation.h>
#define MaxMapW 50
#define MaxMapH 50

class Player: public DynamicObject
{
public:
    long double curX , curY;
    long double realX, realY;
    int number = 0;
    Player(string playerTexturePlace, int startX, int startY, int w, int h, MyMap *MapIn, AllHitboxInf *AHIIn);
    void drawObject(long double &time);
    void displayObject(RenderWindow *window);
    int tryToMove(long double distance, int direction, int mode);
    int getNumber();
    void setNumber(int n);
    pair <long double, long double> coordinates();
    pair <long double, long double> sizes();

private:
    long double scale;
    long double playerSpeed;
    long double heightCoef;
    long double weight;
    long double maxPlayerXSpeed, playerXAcc;
    long double moveSpeed;
    long double freeFallAcc, fallSpeed, maxFallSpeed;
    long double lastCurY;
    int lastDir;

    long double startX, startY;
    long double mapWidth, mapHeight;
    long double height, width;
    long double volume;
    long double startH, startW;
    long double anEX, anEY, anCutTex = 37;
    int anDir;

    AllHitboxInf *AHI;
    int windowWidth, windowHeight;
    bool onGround = false;
    int physics[MaxMapW][MaxMapH], tilesize, ptilesize;
    int levelPassCheck();
    Sprite playerSprite;
    Texture playerTexture;

    void Update(long double &time);
    void animationLevelPass(long double time);

    int isTouching(long double newX, long double newY, int direction);
    int tryToSquezze(long double distance, int direction, int mode);
    char type();
};

#endif

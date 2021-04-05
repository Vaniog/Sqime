#ifndef PLAYER_H
#define PLAYER_H

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <DynamicObject.h>
#include <AllHitboxInf.h>
#include <MyMap.h>
#define MaxMapW 50
#define MaxMapH 50

class Player: public DynamicObject
{
public:
    float curX , curY;
    int number = 0;
    Player(string playerTexturePlace, int startX, int startY, int w, int h, MyMap *MapIn, AllHitboxInf *AHIIn);
    void drawObject(float &time);
    void displayObject(RenderWindow *window);
    int tryToMove(float distance, int direction, int mode);
    int getNumber();
    void setNumber(int n);
    pair <float, float> coordinates();
    pair <float, float> sizes();

private:
    float scale;
    float playerSpeed;
    float heightCoef;
    float weight;
    float maxPlayerXSpeed, playerXAcc;

    float freeFallAcc, fallSpeed, maxFallSpeed;

    float startX, startY;
    float mapWidth, mapHeight;
    float height, width;
    float volume;
    float startH, startW;
    float anEX, anEY, anCutTex = 37;
    int anDir;

    AllHitboxInf *AHI;
    int windowWidth, windowHeight;
    bool onGround = false;
    int physics[MaxMapW][MaxMapH], tilesize, ptilesize;
    int levelPassCheck();
    Sprite playerSprite;
    Texture playerTexture;

    void Update(float &time);
    void animationLevelPass(float time);

    int isTouching(float newX, float newY, int direction);
    int tryToSquezze(float distance, int direction, int mode);
    char type();
};

#endif

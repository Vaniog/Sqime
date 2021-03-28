#ifndef PLATFORM_H
#define PLATFORM_H
#include <SFML/Graphics.hpp>
#include <DynamicObject.h>
#include <MyMap.h>
class MyMap;
class AllHitboxInf;
class Platform: public DynamicObject
{
public:
    float curX, curY;
    float width, height;
    float weight;
    int number;

    Platform(float startXIn, float startYIn, float endXIn, float endYIn, float widthIn, float heightIn, float speed, std::string texPlace, MyMap *MapIn, AllHitboxInf *AHIIn);
    void drawObject(float &time);
    void displayObject(sf::RenderWindow *window);
    std::pair <float, float> coordinates();
    std::pair <float, float> sizes();
    int getNumber();
    void setNumber(int n);
private:
    float startX, startY;
    float endX, endY;
    int dir;
    float scale;
    float tilesize;
    float platformSpeed;
    float mapStartX, mapStartY;
    sf::Texture platformTexture;
    sf::Sprite platformSprite;
    AllHitboxInf *AHI;

    void movePlatform(float time, AllHitboxInf *AHI);
    int tryToMove(float distance, int direction, int mode); // return -1;
    int tryToSquezze(float distance, int direction, int mode);
    char type();
};

#endif // PLATFORM_H

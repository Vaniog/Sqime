#ifndef MYMAP_H_INCLUDED
#define MYMAP_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <AllHitboxInf.h>
#include <Player.h>
#include <Platform.h>
#include <DynamicObject.h>
#define maxMapW 50
#define maxMapH 50

class Player;

class MyMap
{
public:
    float width, height; // in tiles
    float tilesize;
    float scale;
    int startX, startY;
    AllHitboxInf *AHI;
    MyMap *mapAdress = NULL;
    MyMap(std::string mapFilePlace, std::string tilesPlace, std::string backgroundPlace);
    int DrawMap(sf::RenderWindow *window, float time);
    int getPhysics(int x, int y);

private:
    int tiles[maxMapW][maxMapH][2];
    int physics[maxMapW][maxMapH];
    sf::Texture tileTexture, backTex;
    sf::Sprite tilesprite, backSprite;
    int windowWidth, windowHeight;

    void mapDownload (std::string mapFilePlace);
    void physicsCreate();
    float getNumber(std::ifstream &mapFile);
};
#endif

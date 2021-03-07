#ifndef MYMAP_H_INCLUDED
#define MYMAP_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <AllHitboxInf.h>
#include <Player.h>
#include <Platform.h>
#include <DynamicObject.h>
using namespace std;
using namespace sf;
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
    MyMap(string mapFilePlace, string tilesPlace, string backgroundPlace);
    int DrawMap(RenderWindow *window, float time);
    int getPhysics(int x, int y);

private:
    int tiles[maxMapW][maxMapH][2];
    int physics[maxMapW][maxMapH];
    Texture tileTexture, backTex;
    Sprite tilesprite, backSprite;
    int windowWidth, windowHeight;

    void mapDownload (string mapFilePlace);
    void physicsCreate();
    int getNumber(ifstream &mapFile);
};
#endif

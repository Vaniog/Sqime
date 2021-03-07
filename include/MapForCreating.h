#ifndef MAPFORCREATING_H_INCLUDED
#define MAPFORCREATING_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <Button.h>
#define MaxMapW 50
#define MaxMapH 50

using namespace std;
using namespace sf;

class ObjectInf // for storage of dynamic object on field information, some of values could havent meaning
{
public:
    ObjectInf(float w, float h, float sX, float sY, float eX, float eY, float s, char t)
    {
        width = w;
        height = h;
        startX = sX;
        startY = sY;
        endX = eX;
        endY = eY;
        speed = s;
        type = t;
    }
    float width, height;
    float startX, startY;
    float speed, endX, endY;
    int type; // 0-player 1-platform
};

class MapForCreating
{
public:
    float width, height;

    float tilesize;

    MapForCreating(string tilesPlace, string backgroundPlace, string playerPlaceIn, string platformPlaceIn);
    ~MapForCreating();
    void DrawMap(RenderWindow *window, float time); // includes all drawings and checks
    void mapUpload(string mapFilePlace); // draw map from file, but file need to be in maps, and you dont need to write txt
    int mapDownload(string mapFilePlace);

private:
    float spaceForButtons, spaceAroundButtons, spaceForObjButtons; //

    int tiles[MaxMapW][MaxMapH], tilesPhysics[MaxMapW][MaxMapH]; // tiles for textures, tilesphysics for block type

    int curTile = 0;
    Texture tileTexture, backTex;
    string platformPlace, playerPlace;
    Sprite tilesprite, backSprite;
    int windowWidth, windowHeight;
    int startX, startY;
    int backWidth, backHeight;
    float backScaleX, backScaleY;
    float scale;
    float timeAfterPrevClick; // help check double tap
    int prevClick; // help check double tap
    int buttonsAmount;
    int playerX, playerY;
    Button *platformButton = NULL;
    Button *playerButton = NULL;
    vector <vector<Button*>> buttons;
    vector <vector<int>> buttonsActive; // size is [buttonsAmount][2], [0] for active underbutton, [1] for amount of previous underbuttons, [2] for underbuttons amount
    vector <vector<int>> underButtonsFriends; // friendly blocks, this block (connect with underbutton) dont need to build wall with these blocks
    vector <Button*> objSprites;
    vector <ObjectInf*> objects;

    void correctPositions();
    void tileUpdate(int x, int y); // count ID, and set right texture
    int countNeighbours(int x, int y); // count ID for block
    void keyboardCommands();
    void addToObjSprites(int number);
    void displaySprites (RenderWindow *window), displayButtons(RenderWindow * window);
    void clearObjSprites();
    void feelObjSprites(int amount); // help to download map
    int getNumber(ifstream &mapFile);
    pair <int, int> catchMouse(); // help to input size with help of mouse
};
#endif

#ifndef MAPFORCREATING_H_INCLUDED
#define MAPFORCREATING_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <Button.h>
#include <Slider.h>
#include <SizeChooseMenu.h>
#include "DynamicButtonCreatingUI.h"
#include <iostream>
#include <fstream>
#define MaxMapW 50
#define MaxMapH 50

using namespace std;
using namespace sf;

class ObjectInf // for storage of dynamic object on field information, some of values could havent meaning
{
public:
    ObjectInf(char t)
    {
        type = t;
    }
    long double width, height;
    long double startX, startY;
    long double speed, endX, endY;
    int type; // 0-player 1-platform 2 - dynamic button
    int mode, dir, off, on, controlled; //for dynamic button
};




class MapForCreating
{
public:
    long double width, height;

    long double tilesize;

    MapForCreating(string tilesPlace, string backgroundPlace, string playerPlaceIn, string platformPlaceIn, string buttonPlaceIn, string boxPlaceIn);
    ~MapForCreating();
    void DrawMap(RenderWindow *window, long double time); // includes all drawings and checks
    void mapUpload(string mapFilePlace); // draw map from file, but file need to be in maps, and you dont need to write txt
    int mapDownload(string mapFilePlace);

private:
    long double spaceForButtons, spaceAroundButtons, spaceForObjButtons; //

    int tiles[MaxMapW][MaxMapH], tilesPhysics[MaxMapW][MaxMapH]; // tiles for textures, tilesphysics for block type

    int curTile = 0;
    Texture tileTexture, backTex;
    string platformPlace, playerPlace, buttonPlace, boxPlace;
    Sprite tilesprite, backSprite;
    int windowWidth, windowHeight;
    int startX, startY;
    int backWidth, backHeight;
    long double backScaleX, backScaleY;
    long double scale;
    long double timeAfterPrevClick; // help check double tap
    int prevClick; // help check double tap
    int buttonsAmount;
    int playerX, playerY;
    Button *platformButton = NULL;
    Button *playerButton = NULL;
    Button *buttonButton = NULL;
    Button *boxButton = 0;
    vector <vector<Button*>> buttons;
    vector <vector<int>> buttonsActive; // size is [buttonsAmount][2], [0] for active underbutton, [1] for amount of previous underbuttons, [2] for underbuttons amount
    vector <vector<int>> underButtonsFriends; // friendly blocks, this block (connect with underbutton) dont need to build wall with these blocks
    vector <Button*> objSprites;
    vector <ObjectInf*> objects;

    void correctPositions();
    void tileUpdate(int x, int y); // count ID, and set right texture
    int countNeighbours(int x, int y); // count ID for block
    void keyboardCommands(sf::RenderWindow *window);
    void addToObjSprites(int number);
    void displaySprites (RenderWindow *window), displayButtons(RenderWindow * window);
    void clearObjSprites();
    void feelObjSprites(int amount); // help to download map
    void processCheck(RenderWindow *window);
    void buttonsModeSet(int mode); // turn on or off buttons
    long double getNumber(ifstream &mapFile);
    pair <int, int> catchMouse(sf::RenderWindow *window); // help to input size with help of mouse
    //size choose UI
    pair <int, int> process = {0, 0}; // 0 0 for classic, 1 0 for sizes,  1 1 for coordinates (first number is a type)
    ObjectInf *inputObject;
    SizeChooseMenu *sizeChooseUI;
    DynamicButtonCreatingUI *dynamicButtonCreatingUI;
    Texture sliderBackTex;
    Sprite sliderBackSprite;
    Slider *speedSlider;
    Button *okButton;
};
#endif

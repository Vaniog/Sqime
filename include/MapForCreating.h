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
    ObjectInf(char t)
    {
        type = t;
    }
    float width, height;
    float startX, startY;
    float speed, endX, endY;
    int type; // 0-player 1-platform
};


class sizeChooseMenu
{
public:
    sizeChooseMenu()
    {
        Image sizeChooseImage;
        sizeChooseImage.loadFromFile("images\\SizeChooseMenu.png");
        height = VideoMode::getDesktopMode().height / 2;
        width = height;
        scale = width / sizeChooseImage.getSize().x;

        spaceForBorder = width / 8;

        startX = VideoMode::getDesktopMode().width / 2 -  width / 2 + spaceForBorder;
        startY = VideoMode::getDesktopMode().height / 2 -  height / 2 + spaceForBorder;
        endX = startX + width - spaceForBorder * 2;
        endY = startY + height - spaceForBorder * 2;
        sizeChooseClose = new Button(endX + spaceForBorder - scale * 4, startY - spaceForBorder - scale * 4, 0, 0, 9, 9, 9 * scale, 9 * scale, "images\\CloseSizeChoose.png");

        sizeChooseTexture.loadFromImage(sizeChooseImage);
        sizeChooseSprite.setTexture(sizeChooseTexture);
        sizeChooseSprite.setPosition(startX - spaceForBorder, startY - spaceForBorder);
        sizeChooseSprite.setScale(scale, scale);

        tileWidth = (endX - startX - scale) / 5;
        sizeChooseRect = new RectangleShape(Vector2f(tileWidth - scale, tileWidth - scale));
        sizeChooseRect->setOutlineColor(Color(162, 191, 207));
        sizeChooseRect->setOutlineThickness(scale + 1);
        sizeChooseRect->setFillColor(Color(0, 0, 0, 0));
    }

    pair <int, int> display(RenderWindow *window)
    {
        pair <int, int> sizes = {0, 0};
        int mouseCheckReturn = mouseCheck();
        sizeChooseRect->setSize(Vector2f(tileWidth * valueWidth - scale, tileWidth * valueHeight - scale));
        sizeChooseRect->setPosition(startX + scale, startY + scale);
        window->draw(sizeChooseSprite);
        window->draw(*sizeChooseRect);
        if (sizeChooseClose->buttonDisplayAndCheck(window, -1, -1) == 1)
        {
            sizes.first = -1;
            return sizes;
        }
        else if(mouseCheckReturn == 1)
        {
            sizes.first = valueWidth;
            sizes.second = valueHeight;
            return sizes;
        }
        return sizes;
    }
private:
    Texture sizeChooseTexture;
    Sprite sizeChooseSprite;
    float startX, startY, endX, endY, spaceForBorder;
    float width, height, scale;
    int valueWidth = 1, valueHeight = 1;
    float tileWidth;
    Button *sizeChooseClose;
    RectangleShape *sizeChooseRect;

    int mouseCheck()
    {
        float mouseX = Mouse::getPosition().x;
        float mouseY = Mouse::getPosition().y;
        if (mouseX >= startX && mouseX <= endX - scale && mouseY >= startY && mouseY <= endY - scale)
        {
            valueWidth = (mouseX - startX) / tileWidth + 1;
            valueHeight = (mouseY - startY) / tileWidth + 1;
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                while(Mouse::isButtonPressed(Mouse::Left)){};
                return 1;
            }
        }
        return 0;
    }
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
    //size choose UI
    pair <int, int> process = {0, 0}; // 0 0 for drawing, 1 0 for sizes,  1 1 for coordinates (first number is a type)
    ObjectInf *inputObject;
    sizeChooseMenu *sizeChooseUI;
    Texture sliderBackTex;
    Sprite sliderBackSprite;
    Slider *speedSlider;
    Button *okButton;
};
#endif

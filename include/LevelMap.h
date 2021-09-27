#ifndef LEVELMAP_H
#define LEVELMAP_H

#include <Button.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
using namespace std;
using namespace sf;



class LevelMap
{
public:
    LevelMap(int width);

    int display (RenderWindow *window, long double time, long double newX = -1, long double newY = -1);
    int levelPassedAdd(int levelNumber);
    void startShowingNewLevel(int newLevelIn);

    ~LevelMap();
private:
    bool unlockedTiles[7][6];
    vector <vector<pair<int, int>>> mapTilesForLevels;
    vector <vector<int>> levelsUnlockingByLevel;
    vector <int> levelsPassed;
    vector <int> levelsReached;
    vector <pair <int, int>> levelsButtons; // coords of buttons
    vector <pair<int, int>> levelsCoords;

    long double curX = 0, curY = 0, scale;
    int tilesize, startX, startY;
    Texture mapPaperTexture, mapLabyrinthTexture;
    Sprite mapPaperSprite, mapLabyrinthSprite;

    int lastTouched = -1;
    long double lastGamma = 0;
    long double gammaChangeSpeed = 0.6;

    long double showingTimer = 0;
    int newLevel = 0;

    void showPart(int level, RenderWindow *window, long double gamma);

    void resetLabyrinth();
    void resetFiles();
    long double getNumber(ifstream &file);

};

#endif // LEVELMAP_H

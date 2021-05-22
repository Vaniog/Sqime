#include "LevelMap.h"


LevelMap::LevelMap(int width)
{
    // loading reached levels
    ifstream levelsPassedFile("settings//levelsPassed.txt");
    int levelsPassedAmount = getNumber(levelsPassedFile);
    for (int i = 0; i < levelsPassedAmount; i++)
    {
        levelsPassed.push_back(getNumber(levelsPassedFile));
    }
    levelsPassedFile.close();

    // loading which tiles levels are unlocking
    ifstream mapTilesForLevelsFile("settings//mapTilesForLevels.txt");
    int levelsAmount = getNumber(mapTilesForLevelsFile);
    for (int i = 0; i < levelsAmount; i++)
    {
        vector<pair<int, int>> levelInf;
        int unlockedTilesAmount = getNumber(mapTilesForLevelsFile);
        for (int k = 0; k < unlockedTilesAmount; k++)
        {
            pair<int, int> tileCoords;
            tileCoords.first = getNumber(mapTilesForLevelsFile);
            tileCoords.second = getNumber(mapTilesForLevelsFile);
            levelInf.push_back(tileCoords);
        }
        mapTilesForLevels.push_back(levelInf);
    }
    mapTilesForLevelsFile.close();

    // loading levels unlocking by level
    ifstream levelsUnlockingByLevelFile("settings//levelsUnlockingByLevel.txt   ");
    for (int i = 0; i < levelsAmount; i++)
    {
        vector<int> levelsUnlocking;
        int levelsUnlockingAmount = getNumber(levelsUnlockingByLevelFile);
        for (int k = 0; k < levelsUnlockingAmount; k++)
        {
            levelsUnlocking.push_back(getNumber(levelsUnlockingByLevelFile));
        }
        levelsUnlockingByLevel.push_back(levelsUnlocking);
    }

    //loading visible places to unlockedTiles
    for (int i = 0; i < 7; i++)
        for (int k = 0; k < 6; k++)
            unlockedTiles[i][k] = 0;
    for (int i = 0; i < levelsPassedAmount; i++)
    {
        for (int k = 0; k < mapTilesForLevels[levelsPassed[i] - 1].size(); k++)
            unlockedTiles[mapTilesForLevels[levelsPassed[i] - 1][k].first][mapTilesForLevels[levelsPassed[i] - 1][k].second] = 1;
    }
    // loading visible labyrinth and invisible
    tilesize = 9;
    startX = 41, startY = 46; // in pixels


    Image mapLabyrinthFullImg;
    mapLabyrinthFullImg.loadFromFile("images//MapLabyrinth.png");
    scale = width / mapLabyrinthFullImg.getSize().x;

    //loading reached levels
    ifstream levelsReachedFile("settings//levelsReached.txt");
    int levelsReachedAmount = getNumber(levelsReachedFile);
    for (int i = 0; i < levelsReachedAmount; i++)
    {
        levelsReached.push_back(getNumber(levelsReachedFile));
    }
    levelsReachedFile.close();


    //loading coords

    ifstream levelsCoordsFile("settings//levelsCoords.txt");
    levelsAmount = getNumber(levelsCoordsFile);
    for (int i = 0; i < levelsAmount; i++)
    {
        pair <int, int> coords;
        coords.first = getNumber(levelsCoordsFile);
        coords.second = getNumber(levelsCoordsFile);
        levelsCoords.push_back(coords);
    }
    levelsCoordsFile.close();

    //creating buttons of levels
    for (int i = 0; i < levelsReached.size(); i++)
    {
        pair <int, int> levelButton = {levelsCoords[levelsReached[i] - 1].first, levelsCoords[levelsReached[i] - 1].second};
        levelsButtons.push_back(levelButton);
    }


    for (int i = 0; i < levelsReached.size(); i++)
    {
        unlockedTiles[levelsCoords[levelsReached[i] - 1].first][levelsCoords[levelsReached[i] - 1].second] = 1;
    }


    resetLabyrinth();
    // textures to sprite
    mapPaperTexture.loadFromFile("images//mapPaper.png");
    mapPaperSprite.setTexture(mapPaperTexture);
    mapLabyrinthSprite.setScale(scale, scale);
    mapPaperSprite.setScale(scale, scale);

}

int LevelMap::display (RenderWindow *window, float time, float newX, float newY)
{
    if (newX != -1)
        curX = newX, curY = newY;
    float levelChoosed = 0;
    mapPaperSprite.setPosition(curX, curY);
    window->draw(mapPaperSprite);

    float mouseX = (Mouse::getPosition().x - curX) / tilesize / scale - (float)startX / tilesize, mouseY = (Mouse::getPosition().y - curY) / tilesize / scale - (float)startY / tilesize;

    RectangleShape square(Vector2f(tilesize * scale, tilesize * scale));

    for (int i = 0; i < levelsPassed.size(); i++)
    {
        square.setFillColor(Color(230, 181, 96, 255));
        square.setPosition(curX + ((float)startX / tilesize + levelsCoords[levelsPassed[i] - 1].first) * tilesize * scale, curY + ((float)startY / tilesize + levelsCoords[levelsPassed[i] - 1].second) * tilesize * scale);
        window->draw(square);
    }

    for (int i = 0; i < levelsButtons.size(); i++)
    {
        if (levelsButtons[i].first <= mouseX && mouseX <= levelsButtons[i].first + 1)
            if(levelsButtons[i].second <= mouseY && mouseY <= levelsButtons[i].second + 1)
            {
                if (Mouse::isButtonPressed(Mouse::Left))
                    levelChoosed = levelsReached[i];
                if (i == lastTouched)
                {
                    if (lastGamma + gammaChangeSpeed * time <= 255)
                        lastGamma += gammaChangeSpeed * time;
                }
                else
                    lastGamma = 0, lastTouched = i;

                square.setFillColor(Color(74, 103, 119, lastGamma));
                square.setPosition(curX + ((float)startX / tilesize + levelsButtons[i].first) * tilesize * scale, curY + ((float)startY / tilesize + levelsButtons[i].second) * tilesize * scale);
                window->draw(square);
                continue;
            }
        if (i == lastTouched)
            lastTouched = -1, lastGamma = 0;
    }

    mapLabyrinthSprite.setPosition(curX, curY);
    window->draw(mapLabyrinthSprite);

    if (newLevel && showingTimer < 1)
    {
        if (showingTimer < 0.2)
            showPart(newLevel, window, 255);
        else
            showPart(newLevel, window, (1 - (showingTimer - 0.2) / 0.8) * 255);
        showingTimer += time / 4000;
        return 0;
    }
    else
    {
        newLevel = 0;
        showingTimer = 0;
    }
    return levelChoosed;
}

int LevelMap::levelPassedAdd(int levelNumber)
{
    for (int i = 0; i < levelsPassed.size(); i++)
        if (levelNumber == levelsPassed[i])
            return -0;
    levelsPassed.push_back(levelNumber);

    for (int i = 0; i < levelsUnlockingByLevel[levelNumber - 1].size(); i++)
    {
        int tryToReach = levelsUnlockingByLevel[levelNumber - 1][i];
        bool newLevel = 1;
        for (int k = 0; k < levelsReached.size(); k++)
            if(tryToReach == levelsReached[k])
                newLevel = 0;
        if (newLevel)
            levelsReached.push_back(tryToReach);
        pair <int, int> levelButton = {levelsCoords[tryToReach - 1].first,  levelsCoords[tryToReach - 1].second};
        levelsButtons.push_back(levelButton);
    }

    for (int i = 0; i < mapTilesForLevels[levelNumber - 1].size(); i++)
        unlockedTiles[mapTilesForLevels[levelNumber - 1][i].first][mapTilesForLevels[levelNumber - 1][i].second] = 1;
    resetLabyrinth();
    resetFiles();
}

LevelMap::~LevelMap()
{
 //   for(int i = levelsButtons.size() - 1; i >= 0; i--)
   //     delete levelsButtons[i];
}

void LevelMap::resetLabyrinth()
{
    Image mapLabyrinthFullImg, mapLabyrinthImg;
    mapLabyrinthFullImg.loadFromFile("images//MapLabyrinth.png");
    mapLabyrinthImg.create(mapLabyrinthFullImg.getSize().x, mapLabyrinthFullImg.getSize().y, Color(0, 0, 0, 0));
    for (int i = 0; i < 7; i++)
        for (int k = 0; k < 6; k++)
        {
            int downloadX = startX + tilesize * i;
            int downloadY = startY + tilesize * k;
            if(unlockedTiles[i][k] == 1)
                mapLabyrinthImg.copy(mapLabyrinthFullImg, downloadX, downloadY, IntRect(downloadX, downloadY, tilesize, tilesize));
        }

    mapLabyrinthTexture.loadFromImage(mapLabyrinthImg);
    mapLabyrinthSprite.setTexture(mapLabyrinthTexture);
}

void LevelMap::resetFiles()
{
    ofstream levelsReachedFile("settings//levelsReached.txt");
    levelsReachedFile << levelsReached.size() << "\n";
    for (int i = 0; i < levelsReached.size(); i++)
        levelsReachedFile << levelsReached[i] << " ";

    levelsReachedFile.close();

    ofstream levelsPassedFile("settings//levelsPassed.txt");
    levelsPassedFile << levelsPassed.size() << "\n";
    for (int i = 0; i < levelsPassed.size(); i++)
        levelsPassedFile << levelsPassed[i] << " ";

    levelsPassedFile.close();
}

void LevelMap::startShowingNewLevel(int newLevelIn)
{
    bool start = 1;
    for (int i = 0; i < levelsPassed.size(); i++)
        if (levelsPassed[i] == newLevelIn)
            start = 0;
    if (start)
    {
        levelPassedAdd(newLevelIn);
        showingTimer = 0;
        newLevel = newLevelIn;
    }
}

void LevelMap::showPart(int level, RenderWindow *window, float gamma)
{
    int darkerTiles[7][6];
    for (int i = 0; i < 7; i++)
        for (int k = 0; k < 6; k++)
            darkerTiles[i][k] = 0;


    RectangleShape square(Vector2f(tilesize * scale, tilesize * scale));
    square.setFillColor(Color(255, 231, 150, gamma));
    for (int i = 0; i < mapTilesForLevels[level - 1].size(); i++)
        darkerTiles[mapTilesForLevels[level - 1][i].first][mapTilesForLevels[level - 1][i].second] = 1;

    Image mapLabyrinthFullImg, mapLabyrinthImg;
    mapLabyrinthFullImg.loadFromFile("images//MapPaper.png");
    mapLabyrinthImg.create(mapLabyrinthFullImg.getSize().x, mapLabyrinthFullImg.getSize().y, Color(0, 0, 0, 0));
    for (int i = 0; i < 7; i++)
        for (int k = 0; k < 6; k++)
        {
            if(darkerTiles[i][k] == 1)
            {
                square.setPosition(curX + ((float)startX / tilesize + i) * tilesize * scale, curY + ((float)startY / tilesize + k) * tilesize * scale);
                window->draw(square);
            }
        }

}


float LevelMap::getNumber(ifstream &file)
{
    int i;
    float a = 0;
    int afterPoint = 0;
    char s = 10;
    while (s == 10 || s == 32)
        file.get(s);
    while (s != 10 && s != 32)
    {
        if (s == '.')
            afterPoint = 1;
        else
            if (afterPoint == 0)
                a = a * 10 + s - '0';
            else
            {
                float sNumber = s - '0';
                for (i = 0; i < afterPoint; i++)
                    sNumber /= 10;
                a += sNumber;
                afterPoint++;
            }
        file.get(s);
    }
    return a;
}


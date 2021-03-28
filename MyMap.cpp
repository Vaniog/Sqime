#include "MyMap.h"


MyMap::MyMap(std::string mapFilePlace, std::string tilesPlace, std::string backgroundPlace)
{
    AHI = new AllHitboxInf(this);
    mapAdress = this;
    windowWidth = sf::VideoMode::getDesktopMode().width;
    windowHeight = sf::VideoMode::getDesktopMode().height;
    sf::Image tileImage;
    tileImage.loadFromFile(tilesPlace);
    tileImage.createMaskFromColor(sf::Color(254, 254, 254), 0);
    tilesize = tileImage.getSize().y / 16;
    tileTexture.loadFromImage(tileImage);
    tilesprite.setTexture(tileTexture);

    mapDownload(mapFilePlace);

    sf::Image backgroundImage;
    backgroundImage.loadFromFile(backgroundPlace);
    backTex.loadFromFile(backgroundPlace);
    backSprite.setTexture(backTex);
    backSprite.setTextureRect(sf::IntRect(0, 0, tilesize * width, tilesize * height));
    backSprite.setPosition(startX, startY);
    backSprite.setScale(scale, scale);
}

int MyMap::DrawMap(sf::RenderWindow *window, float time)
{
    int i, k;
    (*window).draw(backSprite);

    for (i = 0; i < width; i++)
    {
        for (k = 0; k < height; k++)
        {
            if (tiles[i][k][0] != 0)
            {
                tilesprite.setTextureRect(sf::IntRect(((tiles[i][k][0] - 1) / 16) * tilesize, (tiles[i][k][0] - 1) % 16 * tilesize, tilesize, tilesize));
                tilesprite.setPosition((int)(startX + i * scale  * tilesize), (int)(startY + k * scale * tilesize));
                tilesprite.setScale(scale, scale);
                (*window).draw(tilesprite);
            }
        }
    }


    int action = AHI->drawObjects(window, time, AHI);
    for (i = 0; i < width; i++)
    {
        for (k = 0; k < height; k++)
        {
            if (tiles[i][k][1] != 0)
            {
                tilesprite.setTextureRect(sf::IntRect(((tiles[i][k][1] - 1) / 16) * tilesize, (tiles[i][k][1] - 1) % 16 * tilesize, tilesize, tilesize));
                tilesprite.setPosition((int)(startX + i * scale  * tilesize), (int)(startY + k * scale * tilesize));
                tilesprite.setScale(scale, scale);
                (*window).draw(tilesprite);
            }
        }
    }

    AHI->resetDepth();
    return action;
}

void MyMap::mapDownload (std::string mapFilePlace)
{
    std::ifstream mapFile(mapFilePlace);
    if(!mapFile.is_open())
        std::cout << "Error";

    width = getNumber(mapFile);
    height = getNumber(mapFile);

    int i, k, tile;
    for (k = 0; k < height; k++)
        for (i = 0; i < width; i++)
        {
            tile = getNumber(mapFile);
            tiles[i][k][0] = 0;
            tiles[i][k][1] = tile;
            if (tile <= 16 * 3)
                std::swap(tiles[i][k][0], tiles[i][k][1]);
        }
    physicsCreate();

    scale = std::min((float)windowWidth / tilesize / width, (float)windowHeight / tilesize / height);
    scale -= (scale * tilesize - (int)(scale * tilesize)) / tilesize;
    startX = ((float)windowWidth - scale * tilesize * width) / 2;
    startY = ((float)windowHeight - scale * tilesize * height) / 2;

    AHI = new AllHitboxInf(this);
    int amount = getNumber(mapFile);
    for(i = 0; i < amount; i++)
    {
        int type = getNumber(mapFile);
        switch(type)
        {
        case 0:
        {
            float w, h, sX, sY;
            w = getNumber(mapFile), h = getNumber(mapFile), sX = getNumber(mapFile), sY = getNumber(mapFile);
            DynamicObject *player = NULL;
            player = new Player("images//Slime.png", sX, sY, w, h, this, AHI);
            player->setNumber(AHI->addObject(player));
            break;
        }
        case 1:
        {
            float w, h, sX, sY, eX, eY, speed;
            w = getNumber(mapFile), h = getNumber(mapFile);
            sX = getNumber(mapFile), sY = getNumber(mapFile);
            eX = getNumber(mapFile), eY = getNumber(mapFile);
            speed = getNumber(mapFile);
            DynamicObject *platform = new Platform(sX, sY, eX, eY, w, h, speed, "images//Platform.png", this, AHI);
            platform->setNumber(AHI->addObject(platform));
            break;
        }
        }
    }
    mapFile.close();
}

void MyMap::physicsCreate()
{
    int i, k;
    for (i = 0; i < width; i++)
        for (k = 0; k < height; k++)
        {
            int tile = std::max(tiles[i][k][0], tiles[i][k][1]);
            if (tile >= 1 && tile <= 16)
                physics[i][k] = 1;
            if ((tile >= 17 && tile <= 48) || tile == 0)
                physics[i][k] = 0;
            if (tile >= 49 && tile <= 124)
                physics[i][k] = (tile - 17) / 16;
        }
}

float MyMap::getNumber(std::ifstream &mapFile)
{
    int i;
    float a = 0;
    int afterPoint = 0;
    char s = 10;
    while (s == 10 || s == 32)
        mapFile.get(s);
    while(s != 10 && s != 32)
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
        mapFile.get(s);
    }
    return a;
}


int MyMap::getPhysics(int x, int y)
{
    return physics[x][y];
}

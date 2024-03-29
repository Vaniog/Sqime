#include "MyMap.h"


MyMap::MyMap(string mapFilePlace, string tilesPlace, string backgroundPlace)
{
    AHI = new AllHitboxInf(this);
    mapAdress = this;
    windowWidth = VideoMode::getDesktopMode().width;
    windowHeight = VideoMode::getDesktopMode().height;
    Image tileImage;
    tileImage.loadFromFile(tilesPlace);
    tileImage.createMaskFromColor(Color(254, 254, 254), 0);
    tilesize = tileImage.getSize().y / 16;
    tileTexture.loadFromImage(tileImage);
    tilesprite.setTexture(tileTexture);

    mapDownload(mapFilePlace);

    Image backgroundImage;
    backgroundImage.loadFromFile(backgroundPlace);
    backTex.loadFromFile(backgroundPlace);
    backSprite.setTexture(backTex);
    backSprite.setTextureRect(IntRect(0, 0, tilesize * width, tilesize * height));
    backSprite.setPosition(startX, startY);
    backSprite.setScale(scale, scale);
}

int MyMap::DrawMap(RenderWindow *window, long double time)
{
    int i, k;
    (*window).draw(backSprite);

    for (i = 0; i < width; i++)
    {
        for (k = 0; k < height; k++)
        {
            if (tiles[i][k][0] != 0)
            {
                tilesprite.setTextureRect(IntRect(((tiles[i][k][0] - 1) / 16) * tilesize, (tiles[i][k][0] - 1) % 16 * tilesize, tilesize, tilesize));
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
                tilesprite.setTextureRect(IntRect(((tiles[i][k][1] - 1) / 16) * tilesize, (tiles[i][k][1] - 1) % 16 * tilesize, tilesize, tilesize));
                tilesprite.setPosition((int)(startX + i * scale  * tilesize), (int)(startY + k * scale * tilesize));
                tilesprite.setScale(scale, scale);
                (*window).draw(tilesprite);
            }
        }
    }

    AHI->resetDepth();
    return action;
}

void MyMap::mapDownload (string mapFilePlace)
{
    ifstream mapFile(mapFilePlace);
    if(!mapFile.is_open())
        cout << "Error";

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
                swap(tiles[i][k][0], tiles[i][k][1]);
        }
    physicsCreate();

    scale = min((long double)windowWidth / tilesize / width, (long double)windowHeight / tilesize / height);
    scale -= (scale * tilesize - (int)(scale * tilesize)) / tilesize;
    startX = ((long double)windowWidth - scale * tilesize * width) / 2;
    startY = ((long double)windowHeight - scale * tilesize * height) / 2;

    AHI = new AllHitboxInf(this);
    int amount = getNumber(mapFile);
    for(i = 0; i < amount; i++)
    {
        int type = getNumber(mapFile);
        switch(type)
        {
        case 0:
        {
            long double w, h, sX, sY;
            w = getNumber(mapFile), h = getNumber(mapFile), sX = getNumber(mapFile), sY = getNumber(mapFile);
            DynamicObject *player = NULL;
            player = new Player("images//Slime.png", sX, sY, w, h, this, AHI);
            player->setNumber(AHI->addObject(player));
            break;
        }
        case 1:
        {
            long double w, h, sX, sY, eX, eY, speed;
            w = getNumber(mapFile), h = getNumber(mapFile);
            sX = getNumber(mapFile), sY = getNumber(mapFile);
            eX = getNumber(mapFile), eY = getNumber(mapFile);
            speed = getNumber(mapFile);
            DynamicObject *platform = new Platform(sX, sY, eX, eY, w, h, speed, "images//Platform.png", this, AHI);
            platform->setNumber(AHI->addObject(platform));
            break;
        }
        case 2:
        {
            long double length, sX, sY, dir, controlObj, controlMode, onMode, offMode;
            sX = getNumber(mapFile), sY = getNumber(mapFile);
            length = getNumber(mapFile);
            dir = getNumber(mapFile);
            controlObj = getNumber(mapFile);
            controlMode = getNumber(mapFile);
            offMode = getNumber(mapFile), onMode = getNumber(mapFile);
            DynamicObject *dynamicButton = new DynamicButton(sX, sY, length, dir, controlObj, controlMode, onMode, offMode, this, AHI, "images//DynamicButton.png");
            dynamicButton->setNumber(AHI->addObject(dynamicButton));
            break;
        }
        case 3:
        {
            long double w, h, sX, sY;
            w = getNumber(mapFile), h = getNumber(mapFile), sX = getNumber(mapFile), sY = getNumber(mapFile);
            DynamicObject *box = NULL;
            box = new Box("images//HeavyBox.png", sX, sY, w, h, this, AHI);
            box->setNumber(AHI->addObject(box));
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
            int tile = max(tiles[i][k][0], tiles[i][k][1]);
            if (tile == 0)
            {
                physics[i][k] = 0;
                continue;
            }
            tile = (tile - 1) / 16;

            if (tile == 0)
                physics[i][k] = 5; // 5
            if (tile >= 1 && tile <= 2)
                physics[i][k] = 0;
            if (tile >= 3 && tile <= 6)
                physics[i][k] = tile + 3; // from 6 to 9
            if (tile >= 7 && tile <= 10)
                physics[i][k] = tile - 6; // from 1 to 4
        }
}

long double MyMap::getNumber(ifstream &mapFile)
{
    int i;
    long double a = 0;
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
                long double sNumber = s - '0';
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

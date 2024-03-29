#include "Platform.h"

Platform::Platform(long double startXIn, long double startYIn, long double endXIn, long double endYIn, long double widthIn, long double heightIn, long double speed, string texPlace, MyMap *MapIn, AllHitboxInf *AHIIn)
{
    AHI = AHIIn;
    scale = MapIn->scale;
    tilesize = MapIn->tilesize;
    mapStartX = MapIn->startX, mapStartY = MapIn->startY;
    startX = startXIn, startY = startYIn;
    endX = endXIn, endY = endYIn;
    width = widthIn, height = heightIn;
    weight = 200;
    platformSpeed = speed * 0.00125;
    curX = startX, curY = startY;
    if (startY == endY)
    {
        if (startX != endX)
            dir = 2 - (abs(endX - startX)) / (endX - startX);
        else
            dir = 0;
    }
    else
        dir = 1 + (abs(endY - startY)) / (endY - startY);
    if (startX > endX)
        swap(startX, endX);
    if (startY > endY)
        swap(startY, endY);

    startXMain = startX, startYMain = startY;
    endXMain = endX, endYMain = endY;

    int conturWidth = 7;
    Image platformImage;
    platformImage.loadFromFile(texPlace);
    platformTexture.loadFromImage(platformImage);
    platformSprite.setTexture(platformTexture);

    int texStartX = 0, texStartY = 0, i;
    for (i = 1; i < width; i++)
        texStartX += i * 16;
    for (i = 1; i < height; i++)
        texStartY += i * 16;
    platformSprite.setTextureRect(IntRect(texStartX, texStartY, tilesize * width, height * tilesize));
    platformSprite.setPosition(startX, startY);
    platformSprite.setScale(MapIn->scale * (tilesize * scale * width) / (tilesize * scale * width), MapIn->scale * (tilesize * scale * height) / (tilesize * scale * height));
}

void Platform::drawObject(long double &time)
{
    if (AHI->animationProcess != 1)
        movePlatform(time, AHI);
}

void Platform::displayObject(RenderWindow *window)
{
    platformSprite.setPosition((int)(curX * scale * tilesize + mapStartX), (int)(curY * scale * tilesize + mapStartY));
    window->draw(platformSprite);
}

pair <long double, long double> Platform::coordinates()
{
    pair <long double, long double> xy = {curX, curY};
    return xy;
}


pair <long double, long double> Platform::sizes()
{
    pair <long double, long double> xy = {width, height};
    return xy;
}

int Platform::getNumber()
{
    return number;
}

void Platform::setNumber(int n)
{
    number = n;
}


void Platform::movePlatform(long double time, AllHitboxInf *AHI)
{
    AHI->resetDepth();
    long double oldX = curX, oldY = curY;
    int oldDir = dir;

    curX += (dir % 2) * (2 - dir) * platformSpeed * time;
    curY += ((dir + 1) % 2) * (dir - 1) * platformSpeed * time;

    long double weightToMove = AHI->tryToMoveAll(number, dir, platformSpeed * time, 0);
    if (weightToMove != -1)
    {
        curX = oldX;
        curY = oldY;

        time *= weight / (weightToMove + weight);

        switch (dir % 2)
        {
        case 0:
            curY += ((dir + 1) % 2) * (dir - 1) * platformSpeed * time;
          //  AHI->tryToMoveAll(number, dir, platformSpeed * time, 1);
            if (curY > endY)
            {
                if (oldY <= endY)
                {
                    curY = endY, dir = 0;
                    if (AHI->tryToMoveAll(number, 2, endY - oldY, 0) != -1)
                        AHI->tryToMoveAll(number, 2, endY - oldY, 1);
                    else
                        curY = oldY;
                }
                else
                {
                    curY = oldY - platformSpeed * time;
                    if (AHI->tryToMoveAll(number, 0, platformSpeed * time, 0) != -1)
                        AHI->tryToMoveAll(number, 0, platformSpeed * time, 1);
                    else
                        curY = oldY;
                    dir = 0;
                }
                break;
            }
            if (curY < startY)
            {
                if (oldY >= startY)
                {
                    curY = startY, dir = 2;
                    if (AHI->tryToMoveAll(number, 0, oldY - startY, 0) != -1)
                        AHI->tryToMoveAll(number, 0, oldY - startY, 1);
                    else
                        curY = oldY;
                }
                else
                {
                    curY = oldY + platformSpeed * time;
                    if (AHI->tryToMoveAll(number, 2, platformSpeed * time, 0) != -1)
                        AHI->tryToMoveAll(number, 2, platformSpeed * time, 1);
                    else
                        curY = oldY;
                    dir = 2;
                }
                break;
            }
            AHI->tryToMoveAll(number, dir, platformSpeed * time, 1);
            break;
        case 1:
            curX += (dir % 2) * (2 - dir) * platformSpeed * time;
          //  AHI->tryToMoveAll(number, dir, platformSpeed * time, 1);
            if (curX > endX)
            {
                if (oldX <= endX)
                {
                    curX = endX, dir = 3;
                    if (AHI->tryToMoveAll(number, 1, endX - oldX, 0) != -1)
                        AHI->tryToMoveAll(number, 1, endX - oldX, 1);
                    else
                        curX = oldX;
                }
                else
                {
                    curX = oldX - platformSpeed * time;
                    if (AHI->tryToMoveAll(number, 3, platformSpeed * time, 0) != -1)
                        AHI->tryToMoveAll(number, 3, platformSpeed * time, 1);
                    else
                        curX = oldX;
                    dir = 3;
                }
                break;
            }
            if (curX < startX)
            {
                if (oldX >= startX)
                {
                    curX = startX, dir = 1;
                    if (AHI->tryToMoveAll(number, 3, oldX - startX, 0) != -1)
                        AHI->tryToMoveAll(number, 3, oldX - startX, 1);
                    else
                        curX = oldX;
                }
                else
                {
                    curX = oldX + platformSpeed * time;
                    if (AHI->tryToMoveAll(number, 1, platformSpeed * time, 0) != -1)
                        AHI->tryToMoveAll(number, 1, platformSpeed * time, 1);
                    else
                        curX = oldX;
                    dir = 1;
                }
                break;
            }
            AHI->tryToMoveAll(number, dir, platformSpeed * time, 1);
            break;
        }
    }
    else
    {
        dir = (dir + 2) % 4;
        curX = oldX;
        curY = oldY;
    }
    if (startX == endX && startY == endY && oldX == startX && oldY == startY)
        dir = oldDir;
}

void Platform::sendMessage(long double message)
// modes: 0 - both direction, 1 - plus direction, 2 - minus direction, 3 - stop moving
{
    int mode = message;
    switch (mode)
    {
    case 0:
        startX = startXMain;
        startY = startYMain;
        endX = endXMain;
        endY = endYMain;
        break;
    case 1:
        startX = endXMain;
        startY = endYMain;
        endX = endXMain;
        endY = endYMain;
        break;
    case 2:
        startX = startXMain;
        startY = startYMain;
        endX = startXMain;
        endY = startYMain;
        break;
    case 3:
        startX = curX;
        startY = curY;
        endX = curX;
        endY = curY;
        break;
    }
}
int Platform::tryToMove(long double distance, int direction, int mode)
{
    return -1;
}

int Platform::tryToSquezze(long double distance, int direction, int mode)
{
    return -1;
}

char Platform::type()
{
    return 'm'; // moving platform
}

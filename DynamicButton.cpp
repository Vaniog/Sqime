#include "DynamicButton.h"

DynamicButton::DynamicButton(int curXIn, int curYIn, int length, int direction, MyMap *mapIn, AllHitboxInf *AHIIn, string DynamicButtonPlace)
{
    AHI = AHIIn;
    curX = curXIn;
    curY = curYIn;
    startMapX = mapIn->startX;
    startMapY = mapIn->startY;
    scale = mapIn->scale;
    tilesize = mapIn->tilesize;
    float thickness = (float)5 / 16;
    switch (direction)
    {
    case 0:
        height = thickness;
        width = length;
        endX = curX;
        endY = curY - height;
        break;
    case 1:
        height = length;
        width = thickness;
        curX += 1 - thickness;
        endX = curX + width;
        endY = curY;
        break;
    case 2:
        height = thickness;
        width = length;
        curY += 1 - thickness;
        endX = curX;
        endY = curY + height;
        break;
    case 3:
        height = length;
        width = thickness;
        endX = curX - width;
        endY = curY;
        break;
    }
    startX = curX, startY = curY;
    dbDirection = direction;
    Image dbImage;
    dbImage.loadFromFile(DynamicButtonPlace);
    dbTexture.loadFromImage(dbImage);
    dbSprite.setTexture(dbTexture);
    dbSprite.setScale((width * tilesize + (direction % 2)) / dbImage.getSize().x * scale, (height * tilesize  - (direction % 2 - 1)) / dbImage.getSize().y * scale);
}

DynamicButton::~DynamicButton()
{
}

int DynamicButton::tryToMove(float distance, int direction, int mode)
{
    int weight = 0;
    if (direction == dbDirection)
    {
        switch (direction)
        {
        case 0:
            if (curY > endY)
            {
                if (mode == 1)
                    curY -= distance;
                return weight;
            }
            else
                return - 1;
        case 1:
            if (curX < endX)
            {
                if (mode == 1)
                    curX += distance;
                return weight;
            }
            else
                return -1;
        case 2:
            if (curY < endY)
            {
                if (mode == 1)
                    curY += distance;
                return weight;
            }
            else
                return - 1;
        case 3:
            if (curX > endX)
            {
                if (mode == 1)
                    curX -= distance;
                return weight;
            }
            else
                return -1;
        }
    }
    if (direction != (dbDirection + 2) % 4)
    {
        curX = endX, curY = endY;
        return 0;
    }
    return -1;
}
int DynamicButton::tryToSquezze(float distance, int direction, int mode)
{
    return -1;
}

void DynamicButton::drawObject(float &time)
{
    float distance;
    float oldX = curX;
    float oldY = curY;
    switch (dbDirection)
    {
    case 0:
        if (curY != startY)
        {
            distance = (startY - curY) + 0.01;
            curY = startY;
            if (AHI->tryToMoveAll(number, dbDirection, -distance, 0) != 0)
                curY = oldY;
            }
        break;
    case 1:
        if (curX != startX)
        {
            distance = (curX - startX) + 0.01;
            curX = startX;
            if (AHI->tryToMoveAll(number, dbDirection, -distance, 0) != 0)
                curX = oldX;
        }
        break;
    case 2:
        if (curY != startY)
        {
            distance = (curY - startY) + 0.01;
            curY = startY;
            if (AHI->tryToMoveAll(number, dbDirection, -distance, 0) != 0)
                curY = oldY;
        }
        break;
    case 3:
        if (curX != startX)
        {
            distance = (startX - curX) + 0.01;
            curX = startX;
            if (AHI->tryToMoveAll(number, dbDirection, -distance, 0) != 0)
                curX = oldX;
        }
        break;
    }
}

void DynamicButton::displayObject(RenderWindow *window)
{
    dbSprite.setPosition(startMapX + curX * scale * tilesize, startMapY + curY * scale * tilesize);
    window->draw(dbSprite);
}
char DynamicButton::type()
{
    return 'd';
}

pair <float, float> DynamicButton::coordinates()
{
    pair <float, float> ret = {curX, curY};
    return ret;
}
pair <float, float> DynamicButton::sizes()
{
    pair <float, float> ret = {width, height};
    return ret;
}

int DynamicButton::getNumber()
{
    return number;
}
void DynamicButton::setNumber(int n)
{
    number = n;
}

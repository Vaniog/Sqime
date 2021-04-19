#include "DynamicButton.h"

DynamicButton::DynamicButton(int curXIn, int curYIn, int length, int direction, int controlledObjectIn, int buttonControlModeIn, int onObjMode, int offObjMode, MyMap *mapIn, AllHitboxInf *AHIIn, string DynamicButtonPlace)
{
    AHI = AHIIn;
    curX = curXIn;
    curY = curYIn;

    controlledObject = controlledObjectIn;
    buttonControlMode = buttonControlModeIn;
    modesToSend[0] = offObjMode, modesToSend[1] = onObjMode;
    AHI->sendMessageToObject(controlledObject, modesToSend[0]);

    startMapX = mapIn->startX;
    startMapY = mapIn->startY;
    scale = mapIn->scale;
    tilesize = mapIn->tilesize;
    float thickness = (float)6 / 16;
    switch (direction)
    {
    case 0:
        height = thickness;
        width = length;
        curY -= (float)1 / 16;
        endX = curX;
        endY = curY - height - 0.01;
        break;
    case 1:
        height = length;
        width = thickness;
        curX += 1 - thickness + (float)1 / 16;
        endX = curX + width + 0.01;
        endY = curY;
        break;
    case 2:
        height = thickness;
        width = length;
        curY += 1 - thickness + (float)1 / 16;
        endX = curX;
        endY = curY + height + 0.01;
        break;
    case 3:
        height = length;
        width = thickness;
        curX -= (float)1 / 16;
        endX = curX - width - 0.01;
        endY = curY;
        break;
    }
    startX = curX, startY = curY;
    dbDirection = direction;
    Image dbImage;
    dbImage.loadFromFile(DynamicButtonPlace);
    dbTexture.loadFromImage(dbImage);
    dbSprite.setTexture(dbTexture);
    if (direction % 2 == 0)
    {
        int texStartX = 0, i;
        for (i = 1; i < length; i++)
            texStartX += i * 16;
        dbSprite.setTextureRect(IntRect(texStartX, 0, tilesize * length, 6));
    }
    else
    {
        int texStartY = 0, i;
        for (i = 1; i < length; i++)
            texStartY += i * 16;
        dbSprite.setTextureRect(IntRect(0, texStartY + 6, 6, tilesize * length));
    }
    dbSprite.setScale(scale, scale);
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
            if (curY - distance > endY)
            {
                if (mode == 1)
                    curY -= distance, buttonBecome(1);
                return weight;
            }
            else
                return - 1;
        case 1:
            if (curX + distance < endX)
            {
                if (mode == 1)
                    curX += distance, buttonBecome(1);
                return weight;
            }
            else
                return -1;
        case 2:
            if (curY + distance < endY)
            {
                if (mode == 1)
                    curY += distance, buttonBecome(1);
                return weight;
            }
            else
                return - 1;
        case 3:
            if (curX - distance > endX)
            {
                if (mode == 1)
                    curX -= distance, buttonBecome(1);
                return weight;
            }
            else
                return -1;
        }
    }
    if (direction != (dbDirection + 2) % 4)
    {
        if (mode == 1)
        {
            curX = endX, curY = endY;
            buttonBecome(1);
        }
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
            distance = (startY - curY);
            distance = min(distance, (float)0.02);
            curY += distance;
            if (AHI->tryToMoveAll(number, dbDirection, -distance, 0) != 0)
                curY = oldY;
            else if (curY == startY)
                buttonBecome(0);
            }
        break;
    case 1:
        if (curX != startX)
        {
            distance = (curX - startX);
            distance = min(distance, (float)0.02);
            curX -= distance;
            if (AHI->tryToMoveAll(number, dbDirection, -distance, 0) != 0)
                curX = oldX;
            else if (curX = startX)
                buttonBecome(0);
        }
        break;
    case 2:
        if (curY != startY)
        {
            distance = (curY - startY);
            distance = min(distance, (float)0.02);
            curY -= distance;
            if (AHI->tryToMoveAll(number, dbDirection, -distance, 0) != 0)
                curY = oldY;
            else if (curY == startY)
                buttonBecome(0);
        }
        break;
    case 3:
        if (curX != startX)
        {
            distance = (startX - curX);
            distance = min(distance, (float)0.02);
            curX += distance;
            if (AHI->tryToMoveAll(number, dbDirection, -distance, 0) != 0)
                curX = oldX;
            else if (curX == startX)
                buttonBecome(0);
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

void DynamicButton::buttonBecome(int location)
{
    switch (buttonControlMode)
    {
    case 0:
        if (location == 0)
            AHI->sendMessageToObject(controlledObject, modesToSend[0]);
        else
            AHI->sendMessageToObject(controlledObject, modesToSend[1]);
        break;
    case 1:
        if (location == 0)
            modeToSendNow = 1 - modeToSendNow; // turn 1 to 0 and 0 to 1
        if (location == 1)
            AHI->sendMessageToObject(controlledObject, modesToSend[modeToSendNow]);
        break;


    }
}

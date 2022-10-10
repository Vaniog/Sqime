#include "Box.h"

Box::Box(string boxTexturePlace, int sX, int sY, int w, int h, MyMap *MapIn, AllHitboxInf *AHIIn)
{
    AHI = AHIIn;
    windowWidth = VideoMode::getDesktopMode().width;
    windowHeight = VideoMode::getDesktopMode().height;
    curX = sX;
    curY = sY;
    realY = curY;
    width = w, height = h;
    tilesize = 16;
    ptilesize = 32;
    Image boxImage;
    boxImage.loadFromFile(boxTexturePlace);

    boxTexture.loadFromImage(boxImage);
    boxSprite.setTexture(boxTexture);

    int texStartX = 0, texStartY = 0, i;
    for (i = 1; i < width; i++)
        texStartX += i * 16;
    for (i = 1; i < height; i++)
        texStartY += i * 16;

    boxSprite.setTextureRect(IntRect(texStartX, texStartY, tilesize * width, tilesize * height));
    scale =  MapIn->scale;
    boxSprite.setScale(scale, scale);
    width *= 0.999;
    height *= 0.999;
    weight = 1;
    startX = MapIn->startX;
    startY = MapIn->startY;
    fallSpeed = 0;
    freeFallAcc = 0.00004;
    maxFallSpeed = 0.04;
    onGround = 1;
    mapWidth = MapIn->width;
    mapHeight = MapIn->height;


    int k;
    for (i = 0; i < MapIn->width; i++)
        for (k = 0; k < MapIn->height; k++)
            physics[i][k] = MapIn->getPhysics(i, k);
}

void Box::drawObject(long double &time)
{
    switch (AHI->animationProcess)
    {
    case 0:
        Update(time);
        break;
    case 1:
        break;
    }
}

void Box::displayObject(RenderWindow *window)
{
    if (AHI->animationProcess == 0)
        boxSprite.setPosition(curX * scale * tilesize + startX, startY + realY * scale * tilesize);
    window->draw(boxSprite);
}



void Box::Update(long double &time) // physics moves
{
    AHI->resetDepth();

    long double oldY;
    if (lastCurY == curY)
    {
        if (tryToMove(realY - curY, 2, 0))
            tryToMove(realY - curY, 2, 1);
        else
            curY = oldY;
    }
    else
        realY = curY;

    if (onGround == 1)
    {
        long double weightToMove = tryToMove(0.0001, 2, 0);
        if (weightToMove != -1)
            onGround = 0;
    }
    else
    {
        long double changeY = fallSpeed * time + freeFallAcc * time * time / 2;
        if (fallSpeed > maxFallSpeed)
            fallSpeed = maxFallSpeed;
        long double weightToMove = tryToMove(changeY, 2, 0);
        if (weightToMove == -1)
        {
            if (fallSpeed >= 0)
                onGround = 1;
            else
            {
                changeY = (int)curY - curY;
                if (changeY < 0.02)
                {
                    weightToMove = tryToMove(changeY, 2, 0);
                    if (weightToMove != -1)
                        tryToMove(changeY * weight / weightToMove, 2, 1);
                }
            }
            fallSpeed = 0;
        }
        else
        {
            if (weightToMove <= weight * 2)
                fallSpeed += freeFallAcc * time;
            else
                if (fallSpeed > 0)
                    onGround = 1, fallSpeed += freeFallAcc * weight / weightToMove / 2;
                else
                    fallSpeed += freeFallAcc * time * weightToMove / weight, fallSpeed = min((long double)0, fallSpeed);
            tryToMove(changeY * weight / weightToMove, 2, 1);
        }
    }
    realY = curY;


    if (abs((int)(curY + height + 1) - (curY + height)) < 0.01)
    {
        long double oldY = curY;
        long double changeY = (int)(curY + height + 1) - (curY + height);
        if (tryToMove(changeY, 2, 0))
            tryToMove(changeY, 2, 1);
        else
            curY = oldY;

    }
    else if (abs(curY + height - (int)(curY + height)) < 0.01)
    {
        long double oldY = curY;
        long double changeY = curY + height - (int)(curY + height);
        if (tryToMove(changeY, 0, 0))
            tryToMove(changeY, 0, 1);
        else
            curY = oldY;
    }
    lastCurY = curY;
}


int Box::isTouching(long double newX, long double newY, int direction) // check which block box touch in this direction
{
    int block = 0;
    bool wasStop = 0;
    long double i = -1, k = -1;
    while(i < width)
    {
        i++;
        if (i > width)
            i = width;
        k = -1;
        while (k < height)
        {
            k++;
            if (k > height)
            {
                if (newY + height == (int)(newY + height))
                {
                    continue;
                }
                else
                    k = height;
            }

            if (newX + i < 0 || newX + i >= mapWidth || newY + k < 0 || newY + k >= mapHeight)
                return 1;
            block = physics[(int)(newX + i)][(int)(newY + k)];
            if (block == 0)
                continue;
            if (block >= 1 && block <= 4)
                if (block - 1 == (direction + 2) % 4)
                    wasStop = 1;
            if (block >= 6 && block <= 9)
                return block - 4;
            if (block == 5)
                wasStop = 1;
        }
    }
    if (wasStop)
    {
        return 1;
    }

    return 0;
}

int Box::tryToMove(long double distance, int direction, int mode) // mode 0 for check possible 1 for moving
{
    if (distance < 0)
        direction = (direction + 2) % 4, distance = -distance;

    long double weightToMove = 0;
    long double oldX = curX, oldY = curY;

    switch (direction)
    {
    case 0:
        curY -= distance;
        break;
    case 1:
        curX += distance;
        break;
    case 2:
        curY += distance;
        break;
    case 3:
        curX -= distance;
        break;
    }
    if(mode == 0)
        weightToMove = AHI->tryToMoveAll(number, direction, distance, 0);
    if (weightToMove != -1)
    {
    switch(direction)
    {
    case 0:
    {
        if (isTouching(curX, curY, 0) != 0)
        {
            curX = oldX, curY = oldY;
            return -1;
        }
        if (mode == 1)
        {
            if (AHI->tryToMoveAll(number, direction, distance, 1) == -1)
                curX = oldX, curY = oldY;
        }
        else
            curX = oldX, curY = oldY;
        break;
    }
    case 1:
    {
        if (isTouching(curX, curY, 1) != 0)
        {
            curX = oldX, curY = oldY;
            return -1;
        }
        if (mode == 1)
        {
            if (AHI->tryToMoveAll(number, direction, distance, 1) == -1)
                curX = oldX, curY = oldY;
        }
        else
            curX = oldX, curY = oldY;
        break;
    }
    case 2:
    {
        if (isTouching(curX, curY, 2) != 0)
        {
            curX = oldX, curY = oldY;
            return -1;
        }
        if (mode == 1)
        {
            if (AHI->tryToMoveAll(number, direction, distance, 1) == -1)
                curX = oldX, curY = oldY;
        }
        else
            curX = oldX, curY = oldY;
        break;
    }
    case 3:
    {
        if (isTouching(curX, curY, 3) != 0)
        {
            curX = oldX, curY = oldY;
            return -1;
        }
        if (mode == 1)
        {
            if (AHI->tryToMoveAll(number, direction, distance, 1) == -1)
                curX = oldX, curY = oldY;
        }
        else
            curX = oldX, curY = oldY;
        break;
    }

    }
    }
    else
    {
        curX = oldX;
        curY = oldY;
        return -1;
    }

    if (mode == 0)
        curX = oldX, curY = oldY;
    return weightToMove + weight;
}

int Box::tryToSquezze(long double distance, int direction, int mode)
{
    return -1;
}


pair <long double, long double> Box::coordinates()
{
    pair <long double, long double> xy = {curX, curY};
    return xy;
}

pair <long double, long double> Box::sizes()
{
    pair <long double, long double> xy = {width, height};
    return xy;
}

char Box::type()
{
    return 'b';
}

int Box::getNumber()
{
    return number;
}

void Box::setNumber(int n)
{
    number = n;
}

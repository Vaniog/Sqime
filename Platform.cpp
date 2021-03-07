#include <Platform.h>

Platform::Platform(float startXIn, float startYIn, float endXIn, float endYIn, float widthIn, float heightIn, float speed, string texPlace, MyMap *MapIn, AllHitboxInf *AHIIn)
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
        dir = 2 - (abs(endX - startX)) / (endX - startX);
    else
        dir = 1 + (abs(endY - startY)) / (endY - startY);
    if (startX > endX)
        swap(startX, endX);
    if (startY > endY)
        swap(startY, endY);

    int conturWidth = 7;
    Image platformImage;
    platformImage.loadFromFile(texPlace);
    platformTexture.loadFromImage(platformImage);
    platformSprite.setTexture(platformTexture);
    platformSprite.setTextureRect(IntRect(0, 0, tilesize * width, height * tilesize));
    platformSprite.setPosition(startX, startY);
    platformSprite.setScale(MapIn->scale * (tilesize * scale * width - conturWidth * 2) / (tilesize * scale * width), MapIn->scale * (tilesize * scale * height - conturWidth * 2) / (tilesize * scale * height));
}

void Platform::drawObject(float &time)
{
    if (AHI->animationProcess != 1)
        movePlatform(time, AHI);
}

void Platform::displayObject(RenderWindow *window)
{
    int conturWidth = 7;
    platformSprite.setPosition((int)(curX * scale * tilesize + mapStartX) + conturWidth, (int)(curY * scale * tilesize + mapStartY) + conturWidth);
    RectangleShape contur(Vector2f(width * scale * tilesize - conturWidth * 2, height * scale * tilesize - conturWidth * 2));
    contur.setOutlineThickness(conturWidth);
    contur.setOutlineColor(Color(32, 47, 57));
    contur.setPosition((int)(curX * scale * tilesize + mapStartX) + conturWidth, (int)(curY * scale * tilesize + mapStartY) + conturWidth);
    window->draw(contur);
    window->draw(platformSprite);
}

pair <float, float> Platform::coordinates()
{
    pair <float, float> xy = {curX, curY};
    return xy;
}


pair <float, float> Platform::sizes()
{
    pair <float, float> xy = {width, height};
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


void Platform::movePlatform(float time, AllHitboxInf *AHI)
{
    AHI->resetDepth();
    float oldX = curX, oldY = curY;
    curX += (dir % 2) * (2 - dir) * platformSpeed * time;
    curY += ((dir + 1) % 2) * (dir - 1) * platformSpeed * time;

    float weightToMove = AHI->tryToMoveAll(number, dir, platformSpeed * time, 0);

    if (weightToMove != -1)
    {
        curX = oldX, curY = oldY;

        time *= weight / (weightToMove + weight);
        curX += (dir % 2) * (2 - dir) * platformSpeed * time;
        curY += ((dir + 1) % 2) * (dir - 1) * platformSpeed * time;
        AHI->tryToMoveAll(number, dir, platformSpeed * time, 1);
        if (dir == 1 || dir == 3)
        {
            if (curX < startX)
                dir = 1;
            if (curX > endX)
                dir = 3;
        }
        else
        {
            if (curY < startY)
                dir = 2;
            if (curY > endY)
                dir = 0;
        }
    }
    else
    {
        dir = (dir + 2) % 4;
        curX = oldX;
        curY = oldY;
    }
}

int Platform::tryToMove(float distance, int direction, int mode)
{
    return -1;
}

int Platform::tryToSquezze(float distance, int direction, int mode)
{
    return -1;
}

char Platform::type()
{
    return 'm'; // moving platform
}

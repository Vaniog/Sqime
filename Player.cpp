#include "DynamicObject.h"
#include "Player.h"


Player::Player(string playerTexturePlace, int sX, int sY, int w, int h, MyMap *MapIn, AllHitboxInf *AHIIn)
{
    AHI = AHIIn;
    windowWidth = VideoMode::getDesktopMode().width;
    windowHeight = VideoMode::getDesktopMode().height;
    curX = sX;
    curY = sY;
    tilesize = 16;
    ptilesize = 32;
    Image playerImage;
    playerImage.loadFromFile(playerTexturePlace);
    playerImage.createMaskFromColor(Color(254, 254, 254), 0);
    playerTexture.loadFromImage(playerImage);
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(IntRect(0, 0, ptilesize, ptilesize));
    playerSprite.setScale(scale * width / startW * tilesize, scale * height / startH * tilesize);
    width = w * 0.99;
    height = h * 0.99;
    heightCoef = 1 / (height / ((int)height + 1));
    volume = width * height;
    weight = 100;
    startH = playerImage.getSize().y;
    startW = playerImage.getSize().x / 2;
    startX = MapIn->startX;
    startY = MapIn->startY;
    playerSpeed = 0;
    maxPlayerXSpeed = 0.0035;
    playerXAcc = 0.000004;
    fallSpeed = 0;
    freeFallAcc = 0.00005;
    maxFallSpeed = 0.04;
    onGround = 1;
    mapWidth = MapIn->width;
    mapHeight = MapIn->height;

    scale =  MapIn->scale;

    int i, k;
    for (i = 0; i < MapIn->width; i++)
        for (k = 0; k < MapIn->height; k++)
            physics[i][k] = MapIn->getPhysics(i, k);
}

void Player::drawObject(float &time)
{
    switch (AHI->animationProcess)
    {
    case 0:
        Update(time);
        levelPassCheck();
        break;
    case 1:
        animationLevelPass(time);
        break;
    }
}

void Player::displayObject(RenderWindow *window)
{
    playerSprite.setTexture(playerTexture);
    if (AHI->animationProcess == 0)
        playerSprite.setPosition(curX * scale * tilesize + startX, startY + curY * scale * tilesize);
    playerSprite.setScale(scale * width / startW * tilesize, scale * height / startH * tilesize);
    window->draw(playerSprite);
}



void Player::Update(float &time) // physics moves
{
    AHI->resetDepth();

    if (Keyboard::isKeyPressed(Keyboard::W) && onGround == 1)
        fallSpeed -= sqrt(2 * freeFallAcc * (height * heightCoef + 0.4)), onGround = 0;
    if (onGround == 1)
    {
        float weightToMove = tryToMove(0.0001, 2, 0);
        if (weightToMove != -1)
            onGround = 0;
    }
    else
    {
        float changeY = fallSpeed * time + freeFallAcc * time * time / 2;
        if (fallSpeed > maxFallSpeed)
            fallSpeed = maxFallSpeed;
        float weightToMove = tryToMove(changeY, 2, 0);
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
                    fallSpeed += freeFallAcc * time * weightToMove / weight, fallSpeed = min((float)0, fallSpeed);
            tryToMove(changeY * weight / weightToMove, 2, 1);
        }
    }




    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        if (playerSpeed > -maxPlayerXSpeed * 0.25)
            playerSpeed = -maxPlayerXSpeed * 0.25;
        playerSprite.setTextureRect(IntRect(ptilesize, 0, ptilesize, ptilesize));
        if (playerSpeed - playerXAcc * time >= -maxPlayerXSpeed)
            playerSpeed -= playerXAcc * time;
        else
            playerSpeed = -maxPlayerXSpeed;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D))
    {
        if (playerSpeed < maxPlayerXSpeed * 0.25)
            playerSpeed = maxPlayerXSpeed * 0.25;
        playerSprite.setTextureRect(IntRect(0, 0, ptilesize, ptilesize));
        if (playerSpeed + playerXAcc * time <= maxPlayerXSpeed)
            playerSpeed += playerXAcc * time;
        else
            playerSpeed = maxPlayerXSpeed;
    }
    else
        playerSpeed = 0;
    float weightToMove = -1;
    if (playerSpeed != 0)
    {
        weightToMove = tryToMove(playerSpeed * time, 1, 0);
    }
    if(weightToMove != -1)
    {
        tryToMove(playerSpeed * time * weight / (weightToMove), 1, 1);
    }
    else
        playerSpeed = 0;
}


int Player::isTouching(float newX, float newY, int direction) // check which block player touch in this direction
{
    switch(direction)
    {
    case 0:
    {
        if (newY < 0)
            return 1;
        float i;
        int block = 0;
        for (i = 0; i < width; i++)
            block = max(physics[(int)(newX + i)][(int)newY], block);
        block = max(physics[(int)(newX + width)][(int)newY], block);
        return block;
    }
    case 1:
    {
        if (newX + width > mapWidth)
            return 1;
        float i;
        int block = 0;
        for (i = 0; i < height; i++)
            block = max(physics[(int)(newX + width)][(int)(newY + i)], block);
        block = max(physics[(int)(newX + width)][(int)(newY + height)], block);
        return block;
    }
    case 2:
    {
        if (newY + height > mapHeight)
            return 1;
        float i;
        int block = 0;
        for (i = 0; i < width; i++)
            block = max(physics[(int)(newX + i)][(int)(newY + height)], block);
        block = max(physics[(int)(newX + width)][(int)(newY + height)], block);
        return block;
    }
    case 3:
    {
        if (newX < 0)
            return 1;
        float i;
        int block = 0;
        for (i = 0; i < height; i++)
            block = max(physics[(int)(newX)][(int)(newY + i)], block);
        block = max(physics[(int)(newX)][(int)(newY + height)], block);
        return block;
    }
    }
}

int Player::tryToMove(float distance, int direction, int mode) // mode 0 for check possible 1 for moving
{
    if (distance < 0)
        direction = (direction + 2) % 4, distance = -distance;

    float weightToMove = 0;
    float oldX = curX, oldY = curY;

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

int Player::tryToSquezze(float distance, int direction, int mode)
{
    switch(direction)
    {
    case 0:
    {
        if (height <= 1 / heightCoef)
            return -1;
        float changeW = volume / (height - distance) - width;
        float oldX = curX;
        float weightToMove1, weightToMove2;
        float returnWeight = weight * width * sqrt(width);
        width += changeW;
        height -= distance;

        curX = oldX - changeW / 2;
        if (isTouching(curX, curY, 1) == 0 && isTouching(curX, curY, 3) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 1, changeW / 2, 0);
            weightToMove2 = AHI->tryToMoveAll(number, 3, changeW / 2, 0);

            if(weightToMove1 != -1 && weightToMove2 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 1, changeW / 2, 1), AHI->tryToMoveAll(number, 3, changeW / 2, 1);
                }
                else
                {
                    curX = oldX;
                    width -= changeW;
                    height += distance;
                }
                return returnWeight + weightToMove1 + weightToMove2;
            }
        }

        curX = oldX;
        if (isTouching(curX, curY, 1) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 1, changeW, 0);
            if(weightToMove1 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 1, changeW, 1);
                }
                else
                {
                    width -= changeW;
                    height += distance;
                }
                return returnWeight + weightToMove1;
            }
        }

        curX = oldX - changeW;
        if (isTouching(curX, curY, 3) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 3, changeW, 0);
            if(weightToMove1 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 3, changeW, 1);
                }
                else
                {
                    curX = oldX;
                    width -= changeW;
                    height += distance;
                }
                return returnWeight + weightToMove1;
            }
        }

        curX = oldX;
        width -= changeW;
        height += distance;

        return -1;
    }
    case 2:
    {
        if (height <= 1 / heightCoef)
            return -1;
        float changeW = volume / (height - distance) - width;
        float oldX = curX;
        float weightToMove1, weightToMove2;
        float returnWeight = weight * width * sqrt(width);
        curY += distance;
        width += changeW;
        height -= distance;

        curX = oldX - changeW / 2;
        if (isTouching(curX, curY, 1) == 0 && isTouching(curX, curY, 3) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 1, changeW / 2, 0);
            weightToMove2 = AHI->tryToMoveAll(number, 3, changeW / 2, 0);
            if(weightToMove1 != -1 && weightToMove2 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 1, changeW / 2, 1), AHI->tryToMoveAll(number, 3, changeW / 2, 1);
                }
                else
                {
                    curX = oldX;
                    width -= changeW;
                    height += distance;
                    curY -= distance;
                }
                return returnWeight + weightToMove1 + weightToMove2;
            }
        }

        curX = oldX;
        if (isTouching(curX, curY, 1) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 1, changeW, 0);
            if(weightToMove1 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 1, changeW, 1);
                }
                else
                {
                    width -= changeW;
                    height += distance;
                    curY -= distance;
                }
                return returnWeight + weightToMove1;
            }
        }

        curX = oldX - changeW;
        if (isTouching(curX, curY, 3) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 3, changeW, 0);
            if(weightToMove1 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 3, changeW, 1);
                }
                else
                {
                    curX = oldX;
                    width -= changeW;
                    height += distance;
                    curY -= distance;
                }
                return returnWeight + weightToMove1;
            }
        }

        curX = oldX;
        width -= changeW;
        height += distance;
        curY -= distance;

        return -1;
    }

    case 1:
    {
        if (width <= 1 / heightCoef)
            return -1;
        float changeH = volume / (width - distance) - height;
        float oldY = curY;
        float weightToMove1, weightToMove2;
        float returnWeight = weight * height * sqrt(height);
        curX += distance;
        height += changeH;
        width -= distance;


        curY = oldY - changeH;
        if (isTouching(curX, curY, 0) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 0, changeH, 0);
            if(weightToMove1 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 0, changeH, 1);
                }
                else
                {
                    height -= changeH;
                    width += distance;
                    curX -= distance;
                    curY = oldY;
                }
                return returnWeight + weightToMove1;
            }
        }

        curY = oldY - changeH / 2;
        if (isTouching(curX, curY, 2) == 0 && isTouching(curX, curY, 0) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 0, changeH / 2, 0);
            weightToMove2 = AHI->tryToMoveAll(number, 2, changeH / 2, 0);
            if(weightToMove1 != -1 && weightToMove2 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 0, changeH / 2, 1), AHI->tryToMoveAll(number, 2, changeH / 2, 1);
                }
                else
                {
                    height -= changeH;
                    width += distance;
                    curX -= distance;
                    curY = oldY;
                }

                return returnWeight + weightToMove1 + weightToMove2;
            }
        }


        curY = oldY;
        if (isTouching(curX, curY, 2) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 2, changeH, 0);
            if(weightToMove1 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 2, changeH, 1);
                }
                else
                {
                    height -= changeH;
                    width += distance;
                    curX -= distance;
                }
                return returnWeight + weightToMove1;
            }
        }

        height -= changeH;
        width += distance;
        curY = oldY;
        curX -= distance;
        return -1;
    }
    case 3:
    {
        if (width <= 1 / heightCoef)
            return -1;
        float changeH = volume / (width - distance) - height;
        float oldY = curY;
        float weightToMove1, weightToMove2;
        float returnWeight = weight * height * sqrt(height);
        height += changeH;
        width -= distance;

        curY = oldY - changeH / 2;
        if (isTouching(curX, curY, 2) == 0 && isTouching(curX, curY, 0) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 0, changeH / 2, 0);
            weightToMove2 = AHI->tryToMoveAll(number, 2, changeH / 2, 0);
            if(weightToMove1 != -1 && weightToMove2 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 0, changeH / 2, 1), AHI->tryToMoveAll(number, 2, changeH / 2, 1);
                }
                else
                {
                    height -= changeH;
                    width += distance;
                    curY = oldY;
                }

                return returnWeight + weightToMove1 + weightToMove2;
            }
        }

        curY = oldY;
        if (isTouching(curX, curY, 2) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 2, changeH, 0);
            if(weightToMove1 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 2, changeH, 1);
                }
                else
                {
                    height -= changeH;
                    width += distance;
                }
                return returnWeight + weightToMove1;
            }
        }

        curY = oldY - changeH;
        if (isTouching(curX, curY, 0) == 0)
        {
            weightToMove1 = AHI->tryToMoveAll(number, 0, changeH, 0);
            if(weightToMove1 != -1)
            {
                if (mode == 1)
                {
                    AHI->tryToMoveAll(number, 0, changeH, 1);
                }
                else
                {
                    height -= changeH;
                    width += distance;
                    curY = oldY;
                }
                return returnWeight + weightToMove1;
            }
        }
        height -= changeH;
        width += distance;
        curY = oldY;
        return -1;
    }
}
}

int Player::levelPassCheck() // check if player passed level
{
    int i;
    float aD = 0.1; // around distance, its what we add to hitbox around
    float sD = 0.1; // side distance, its distance, on what we need neutral block under point (if we watch direction 2)
    float sD2 = 0.1; // assumption in side

    float x[4] = {curX, curX + width, curX + width, curX};
    float y[4] = {curY, curY, curY + height, curY + height};
    float x2[16], y2[16];
    {
    x2[0] = x[0] - sD, x2[1] = x[0] + sD2, x2[2] = x[1] - sD2, x2[3] = x[1] + sD;
    for (i = 0; i < 4; i++)
        y2[i] = y[0] - aD;                                          // up side

    for (i = 4; i < 8; i++)
        x2[i] = x[1] + aD;
    y2[4] = y[1] - sD, y2[5] = y[1], y2[6] = y[2], y2[7] = y[2] + sD;      // right side

    x2[8] = x[0] - sD, x2[9] = x[0] + sD2, x2[10] = x[1] - sD2, x2[11] = x[1] + sD;
    for (i = 8; i < 12; i++)
        y2[i] = y[2] + aD;                                                  // down side

    for (i = 12; i < 16; i++)
        x2[i] = x[0] - aD;
    y2[12] = y[1] - sD, y2[13] = y[1], y2[14] = y[2], y2[15] = y[2] + sD;        // left side
    }

    int rem = -1;
    for (i = 0; i < 16; i += 4)
    {
        int passBlock = (i / 4 + 2) % 4 + 2;
        if (physics[(int)x2[i]][(int)y2[i]] != passBlock && physics[(int)x2[i + 1]][(int)y2[i + 1]] == passBlock && physics[(int)x2[i + 2]][(int)y2[i + 2]] == passBlock && physics[(int)x2[i + 3]][(int)y2[i + 3]] != passBlock)
            rem = i / 4;
    }
    if (rem != -1)
    {
        anDir = rem;
        switch(rem)
        {
        case 0:
            anEX = curX, anEY = curY - height - 0.250;
            break;
        case 1:
            anEX = curX + width + 0.250, anEY = curY;
            break;
        case 2:
            anEX = curX, anEY = curY + height + 0.250;
            break;
        case 3:
            anEX = curX - width - 0.250, anEY = curY;
            break;
        }
        AHI->animationProcess = 1;
    }

    return 0;
}

void Player::animationLevelPass(float time) // if you pass level, program call this function
{
    switch (anDir)
    {
    case 0:
    {
        curX = (int)(curX + 0.1);
        float plungeSpeed = 0.0005 * height;
        if (curY < anEY)
        {
            AHI->animationProcess = 2;
        }
        curY -= plungeSpeed * time;
        anCutTex -= plungeSpeed * time * (float)tilesize * 2 / height;
        playerSprite.setTextureRect(IntRect(0, tilesize * 2 - (int)min(anCutTex, (float)tilesize * 2), tilesize * 2, (int)min(anCutTex, (float)tilesize * 2)));
        playerSprite.setPosition(curX * scale * tilesize + startX, startY + curY * scale * tilesize + (tilesize * 2 - (int)min(anCutTex, (float)32)) * scale * height / 2);
        break;
    }

    case 1:
    {
        curY = (int)(curY + 0.1) + (int)(height + 0.1) - height;
        float plungeSpeed = 0.0005 * width;
        if (curX > anEX)
        {
            AHI->animationProcess = 2;
        }
        curX += plungeSpeed * time;
        anCutTex -= plungeSpeed * time * (float)tilesize * 2 / width;
        playerSprite.setTextureRect(IntRect(0, 0, min(anCutTex, (float)tilesize * 2), tilesize * 2));
        playerSprite.setPosition(curX * scale * tilesize + startX, startY + curY * scale * tilesize);
        break;
    }


    case 2:
    {
        curX = (int)(curX + 0.1);
        float plungeSpeed = 0.0005 * height;
        if (curY > anEY)
        {
            AHI->animationProcess = 2;
        }
        curY += plungeSpeed * time;
        anCutTex -= plungeSpeed * time * (float)tilesize * 2 / height;
        playerSprite.setTextureRect(IntRect(0, 0, tilesize * 2, min(anCutTex, (float)tilesize * 2)));
        playerSprite.setPosition(curX * scale * tilesize + startX, startY + curY * scale * tilesize);
        break;
    }

    case 3:
    {
        curY = (int)(curY + 0.1) + (int)(height + 0.1) - height;
        float plungeSpeed = 0.0005 * width;
        if (curX < anEX)
        {
            AHI->animationProcess = 2;
        }
        curX -= plungeSpeed * time;
        anCutTex -= plungeSpeed * time * (float)tilesize * 2 / width;
        playerSprite.setTextureRect(IntRect(tilesize * 2 - (int)min(anCutTex, (float)tilesize * 2), 0, (int)min(anCutTex, (float)tilesize * 2), tilesize * 2));
        playerSprite.setPosition(curX * scale * tilesize + startX + (tilesize * 2 - (int)min(anCutTex, (float)32)) * scale * width / 2, startY + curY * scale * tilesize);
        break;
    }
    }
}

pair <float, float> Player::coordinates()
{
    pair <float, float> xy = {curX, curY};
    return xy;
}

pair <float, float> Player::sizes()
{
    pair <float, float> xy = {width, height};
    return xy;
}

char Player::type()
{
    return 'p';
}

int Player::getNumber()
{
    return number;
}
void Player::setNumber(int n)
{
    number = n;
}

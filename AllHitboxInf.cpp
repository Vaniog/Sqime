#include "Player.h"

AllHitboxInf::AllHitboxInf(MyMap *mapIn)
{
    mainMap = mapIn;
    depth = 0;
}

int AllHitboxInf::addObject(DynamicObject *obj)
{
    objects.push_back(obj);
    objAmount++;
    return objAmount - 1;
}

int AllHitboxInf::tryToMoveAll(int number, int direction, float distance, int mode)
{
    depth++;
    if (depth > 3000)
    {
        return -1;
    }

    if (distance < 0)
        direction = (direction + 2) % 4, distance = -distance;

    int i;
    pair <float, float> sXY = objects[number]->coordinates(); // start X Y, end X Y
    pair <float, float> eXY = objects[number]->sizes();
    eXY.first += sXY.first;
    eXY.second += sXY.second;
    pair <float, float> realSXY = sXY, realEXY = eXY;

    switch (direction)
    {
    case 0:
        eXY.second = sXY.second;
        break;
    case 1:
        sXY.first = eXY.first;
        break;
    case 2:
        sXY.second = eXY.second;
        break;
    case 3:
        eXY.first = sXY.first;
        break;
    }


    float weight = 0;
    pair <float, float> sXY2;
    pair <float, float> eXY2;
    for (i = 0; i < objAmount; i++)
        if (i != number)
        {
            sXY2 = objects[i]->coordinates();
            eXY2 = objects[i]->sizes();

            eXY2.first += sXY2.first;
            eXY2.second += sXY2.second;

            if (!(sXY.first < eXY2.first && sXY.second < eXY2.second))
            {
                if (realSXY.first <= eXY2.first - 0.1 && sXY2.first <= realEXY.first - 0.1 && eXY2.second <= realSXY.second && eXY2.second + 0.05 >= realSXY.second)
                    if(objects[number]->type() != 'p')
                        if (objects[i]->type() != 'm' || objects[number]->type() != 'm')
                            objects[i]->tryToMove(distance, direction, mode);
                continue;
            }

            if (!(eXY.first > sXY2.first && eXY.second > sXY2.second))
            {
                continue;
            }
            float deltaWeight;
           // cout << number << " " << i << " " << direction << " " << distance << " " << mode << "\n";
            deltaWeight = objects[i]->tryToMove(distance, direction, 0);
            if (deltaWeight == -1)
            {
                deltaWeight = objects[i]->tryToSquezze(distance, direction, 0);
                if (deltaWeight == -1)
                {
                   // cout << "return -1\n";
                    return -1;
                }
                else
                {
                    weight += deltaWeight;
                    if (mode == 1)
                        objects[i]->tryToSquezze(distance, direction, 1);
                }
            }
            else
            {
                weight += deltaWeight;
                if (mode == 1)
                    objects[i]->tryToMove(distance, direction, 1);
            }
        }
        return weight;
}

int AllHitboxInf::drawObjects(RenderWindow *window, float time, AllHitboxInf *AHI)
{
    int i;
    for (i = 0; i < objAmount; i++)
    {

        objects[i]->drawObject(time);
    }
    for (i = 0; i < objAmount; i++)
    {
        objects[i]->displayObject(window);
    }

    return animationProcess;
}

void AllHitboxInf::sendMessageToObject(int number, int mode)
{
    objects[number]->sendMessage(mode);
}
void AllHitboxInf::resetDepth()
{
    depth = 0;
}

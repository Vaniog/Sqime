#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H


#include <bits/stdc++.h>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

using namespace std;
using namespace sf;

class DynamicObject
{
public:
    DynamicObject();
    ~DynamicObject();
    virtual char type() = 0;
    virtual pair <float, float> coordinates() = 0;
    virtual pair <float, float> sizes() = 0;

    virtual int tryToMove(float distance, int direction, int mode) = 0;
    virtual int tryToSquezze(float distance, int direction, int mode) = 0;
    virtual void drawObject(float &time) = 0;
    virtual void displayObject(RenderWindow *window) = 0;

    virtual int getNumber() = 0; // help you found objects in array of all objects (AllHitboxInformation)
    virtual void setNumber(int n) = 0;
};

#endif // DYNAMICOBJECT_H

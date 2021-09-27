#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H


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
    virtual pair <long double, long double> coordinates() = 0;
    virtual pair <long double, long double> sizes() = 0;

    virtual int tryToMove(long double distance, int direction, int mode) = 0;
    virtual int tryToSquezze(long double distance, int direction, int mode) = 0;
    virtual void drawObject(long double &time) = 0;
    virtual void displayObject(RenderWindow *window) = 0;
    virtual void sendMessage(long double message);

    virtual int getNumber() = 0; // help you found objects in array of all objects (AllHitboxInformation)
    virtual void setNumber(int n) = 0;
};

#endif // DYNAMICOBJECT_H

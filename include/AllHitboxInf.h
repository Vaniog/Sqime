#ifndef ALLHITBOXINF_H
#define ALLHITBOXINF_H
#include "DynamicObject.h"

class MyMap;
class AllHitboxInf
{
public:
    AllHitboxInf(MyMap *mapIn);
    int addObject(DynamicObject *obj);
    int tryToMoveAll(int number, int direction, float distance, int mode); // mode 0 for trying, 1 for moving
    int drawObjects(RenderWindow *window, float time, AllHitboxInf *AHI);
    void resetDepth();

    void sendMessageToObject(int number, float message);
    int animationProcess = 0; // 0 - level passing, 1 - level passed animation 2 - level passed
    MyMap *mainMap;
private:
    int objAmount = 0;
    vector <DynamicObject*> objects;
    int depth;
};

#endif // ALLHITBOXINF_H

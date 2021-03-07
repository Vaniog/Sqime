#include "MapForCreating.h"

 MapForCreating::MapForCreating(string tilesPlace, string backgroundPlace, string playerPlaceIn, string platformPlaceIn)
{
    int i, k;
    for (i = 0; i < MaxMapH; i++)
        for (k = 0; k < MaxMapW; k++)
            tiles[k][i] = 0;
    for (i = 0; i < MaxMapH; i++)
        for (k = 0; k < MaxMapW; k++)
            tilesPhysics[k][i] = 0;
    width = 10;
    height = 10;
    windowWidth = VideoMode::getDesktopMode().width;
    windowHeight = VideoMode::getDesktopMode().height;
    Image tileImage;
    tileImage.loadFromFile(tilesPlace);
    tileImage.createMaskFromColor(Color(254, 254, 254), 0);
    tilesize = 16;
    tileTexture.loadFromImage(tileImage);
    tilesprite.setTexture(tileTexture);

    spaceForButtons = 70;
    spaceForObjButtons = 200;
    spaceAroundButtons = 5;

    scale = min(((float)windowWidth - spaceForButtons * 10) / tilesize / width, (float)windowHeight / tilesize / height);

    startX = spaceForButtons * 5 + (((float)windowWidth - spaceForButtons * 10) - tilesize * scale * width) / 2;
    startY = ((float)windowHeight - scale * tilesize * height) / 2;
    scale -= (scale * tilesize - (int)(scale * tilesize)) / tilesize; //scale * tilesize need to be int

    Image backgroundImage;
    backgroundImage.loadFromFile(backgroundPlace);
    backWidth = backgroundImage.getSize().x;
    backHeight = backgroundImage.getSize().y;

    backTex.loadFromFile(backgroundPlace);
    backSprite.setTexture(backTex);
    backSprite.setTextureRect(IntRect(0, 0, tilesize * scale * width, tilesize * scale * height));
    backSprite.setPosition(startX, startY);


    ifstream mapFile("settings\\tilesButtonsInformation.txt");
    if(!mapFile.is_open())
        cout << "Error";

    buttonsAmount = getNumber(mapFile);
    int allUnderbuttonsAmount = 0;
    for (i = 0; i < buttonsAmount; i++)
    {
        vector <Button*> a;
        buttons.push_back(a);
        int underbuttonsAmount = getNumber(mapFile);
        for (k = 0; k < underbuttonsAmount; k++)
        {
            int tileX = getNumber(mapFile), tileY = getNumber(mapFile);
            Button *button = new Button(spaceAroundButtons, 100 + spaceForButtons * i, tileX * tilesize, tileY * tilesize, tilesize, tilesize, spaceForButtons - spaceAroundButtons * 2, spaceForButtons - spaceAroundButtons * 2, "images\\Tile7.png");
            buttons[i].push_back(button);
            int friendsAmount = getNumber(mapFile);
            int j;
            vector <int> friends;
            for (j = 0; j < friendsAmount; j++)
            {
                friends.push_back(getNumber(mapFile));
            }
            underButtonsFriends.push_back(friends);
        }
        vector <int> b = {0, allUnderbuttonsAmount, underbuttonsAmount};
        allUnderbuttonsAmount += underbuttonsAmount;
        buttonsActive.push_back(b);
    }
    mapFile.close();

    playerPlace = playerPlaceIn;
    platformPlace = platformPlaceIn;

    playerButton = new Button(windowWidth - spaceForObjButtons, 200, 0, 0, -1, -1, spaceForObjButtons - spaceAroundButtons * 2, spaceForObjButtons - spaceAroundButtons * 2, "images\\PlayerButton.png");
    platformButton = new Button(windowWidth - spaceForObjButtons, 200 + spaceForObjButtons, 0, 0, -1, -1, spaceForObjButtons - spaceAroundButtons * 2, spaceForObjButtons - spaceAroundButtons * 2, "images\\PlatformButton.png");
 //   ObjectInf *obj = new ObjectInf(2, 2, 0, 0, 0, 0, 0, 's');
  //  objects.push_back(obj);
}

MapForCreating::~MapForCreating()
{
    int i;
    int k;
    clearObjSprites();
    for (i = buttonsAmount - 1; i >= 0; i--)
        for (k = buttonsActive[i][2] - 1; k >= 0; k--)
            delete buttons[i][k];
    for (i = objects.size() - 1; i >= 0; i--)
        delete objects[i];
}

void MapForCreating::DrawMap(RenderWindow *window, float time)
{
    int i, k;
    timeAfterPrevClick += time;
    (*window).draw(backSprite);
    keyboardCommands();

    for (i = 0; i < width; i++)
    {
        for (k = 0; k < height; k++)
        {
            if (tiles[i][k] != 0)
            {
                tilesprite.setTextureRect(IntRect((tiles[i][k] - 1) / 16 * tilesize, (tiles[i][k] - 1) % 16 * (int)tilesize, tilesize, tilesize));
                tilesprite.setPosition((int)(startX + i * scale  * tilesize), (int)(startY + k * scale * tilesize));
                tilesprite.setScale(scale, scale);
                (*window).draw(tilesprite);
            }
        }
    }


    displayButtons(window);
  /*  for (i = 0; i < buttonsAmount; i++)
    {
        if (buttons[i][0]->ButtonDisplayAndCheck(window, spaceAroundButtons, 100 + i * spaceForButtons + spaceAroundButtons) == 1)
        {
            if (curTiles != i)
                curTiles = i;
            else
                curTiles = -1;
        }

        if (curTiles == i)
        {
            int k;
            for (k = 1; k < 17; k++)
                if (buttons[i][k]->ButtonDisplayAndCheck(window, spaceForButtons + spaceAroundButtons + (k - 1) / 4 * spaceForButtons, 100 + i * spaceForButtons + spaceAroundButtons + ((k - 1) % 4) * spaceForButtons) == 1)
                    curTile = i * 16 + k;
        }
    }*/

    if (playerButton->buttonDisplayAndCheck(window, -1, -1))
    {
        int w, h, sX, sY;
        pair <int, int> helper;
        helper = catchMouse();
        w = helper.first;
        h = helper.second;
        helper = catchMouse();
        sX = helper.first;
        sY = helper.second;
        ObjectInf *obj  = new ObjectInf(w, h, sX, sY, 0, 0, 0, 0); //0 - slime
        objects.push_back(obj);
        addToObjSprites(objects.size() - 1);
    }


    if (platformButton->buttonDisplayAndCheck(window, -1, -1))
    {
        float w, h, sX, sY, eX, eY, speed;
        pair <int, int> helper;
        helper = catchMouse();
        w = helper.first;
        h = helper.second;
        helper = catchMouse();
        sX = helper.first;
        sY = helper.second;
        helper = catchMouse();
        eX = helper.first;
        eY = helper.second;
        helper = catchMouse();
        speed = helper.first;
        ObjectInf *obj  = new ObjectInf(w, h, sX, sY, eX, eY, speed, 1); //1 - platform
        objects.push_back(obj);
        addToObjSprites(objects.size() - 1);

    }

    displaySprites(window);
    Font font;
    font.loadFromFile("images\\mainFont.ttf");
    Text infText("", font, 20);
    infText.setColor(Color::White);
    infText.setPosition(10 + windowWidth - spaceForObjButtons, 10);
    string s = to_string((int)width) + "    " + to_string((int)height) + "\n" + to_string((int)((Mouse::getPosition().x - startX) / tilesize / scale)) + "     " +  to_string((int)((Mouse::getPosition().y - startY) / tilesize / scale))
    + "\n\nD - download \nU - upload\nT - try\nN - new\n";
    infText.setString(s);
    window->draw(infText);
}

void MapForCreating::mapUpload (string mapFilePlace)
{
    ofstream mapFile(mapFilePlace);
    if(!mapFile.is_open())
        cout << "FileError\n";
    mapFile << width << "\n";
    mapFile << height << "\n";
    int i, k;
    for (k = 0; k < height; k++)
    {
        for (i = 0; i < width; i++)
            mapFile << tiles[i][k] << " ";
        mapFile << "\n";
    }

    mapFile << objects.size() << "\n";
    for (i = 0; i < objects.size(); i++)
    {
        mapFile << objects[i]->type << " ";
        switch(objects[i]->type)
        {
        case 0:
        {
            mapFile << objects[i]->width << " ";
            mapFile << objects[i]->height << " ";
            mapFile << objects[i]->startX << " ";
            mapFile << objects[i]->startY << "\n";
            break;
        }
        case 1:
        {

            mapFile << objects[i]->width << " ";
            mapFile << objects[i]->height << " ";
            mapFile << objects[i]->startX << " ";
            mapFile << objects[i]->startY << " ";
            mapFile << objects[i]->endX << " ";
            mapFile << objects[i]->endY << " ";
            mapFile << objects[i]->speed << "\n";
            break;
        }
        }
    }

    mapFile.close();
}

int MapForCreating::mapDownload (string mapFilePlace)
{
    int i;
    int k;
    for (i = objects.size() - 1; i >= 0; i--)
    {
        delete objects[i];
        objects.pop_back();
    }
    clearObjSprites();

    ifstream mapFile(mapFilePlace);
    if(!mapFile.is_open())
        cout << "Error";

    width = getNumber(mapFile);
    if (width == -1)
    {
        width = 10;
        return 0;
    }
    height = getNumber(mapFile);

    for (k = 0; k < height; k++)
        for (i = 0; i < width; i++)
        {
            tiles[i][k] = getNumber(mapFile);
            if (tiles[i][k] != 0)
                tilesPhysics[i][k] = (tiles[i][k] - 1) / 16 + 1;
            else
                tilesPhysics[i][k] = 0;
        }

    int amount = getNumber(mapFile);

    for(i = 0; i < amount; i++)
    {
        int type = getNumber(mapFile);
        switch(type)
        {
        case 0:
        {
            float w, h, sX, sY;
            w = getNumber(mapFile), h = getNumber(mapFile), sX = getNumber(mapFile), sY = getNumber(mapFile);
            ObjectInf *obj  = new ObjectInf(w, h, sX, sY, 0, 0, 0, 0); //0 - slime
            objects.push_back(obj);
            break;
        }
        case 1:
        {
            float w, h, sX, sY, eX, eY, speed;
            w = getNumber(mapFile), h = getNumber(mapFile);
            sX = getNumber(mapFile), sY = getNumber(mapFile);
            eX = getNumber(mapFile), eY = getNumber(mapFile);
            speed = getNumber(mapFile);
            ObjectInf *obj  = new ObjectInf(w, h, sX, sY, eX, eY, speed, 1); //1 - platform
            objects.push_back(obj);
            break;
        }
        }
    }

    feelObjSprites(amount);
    mapFile.close();
    correctPositions();
    return 1;
}


void MapForCreating::displaySprites(RenderWindow *window)
{
    int i;
    for (i = 0; i < objSprites.size(); i++)
    {
        if (objSprites[i]->buttonDisplayAndCheck(window, -1, -1) == 2)
        {
            delete objSprites[i];
            objSprites.erase(objSprites.begin() + i);
            delete objects[i];
            objects.erase(objects.begin() + i);
        }
    }
}


void MapForCreating::displayButtons(RenderWindow *window)
{
    int i;
    for (i = 0; i < buttonsAmount; i++)
    {
        if (buttons[i][buttonsActive[i][0]]->buttonDisplayAndCheck(window, -1, -1) == 1)
        {
            if (timeAfterPrevClick < 500 && prevClick == buttonsActive[i][1] + buttonsActive[i][0])
                prevClick = buttonsActive[i][1] + buttonsActive[i][0], buttonsActive[i][0] = (buttonsActive[i][0] + 1) % buttonsActive[i][2];
            else
                prevClick = buttonsActive[i][1] + buttonsActive[i][0];
            curTile = buttonsActive[i][1] + buttonsActive[i][0];
            timeAfterPrevClick = 0;
        }
    }
}

void MapForCreating::clearObjSprites()
{
    int i;
    for (i = objSprites.size() - 1; i >= 0; i--)
        delete objSprites[i], objSprites.pop_back();
}

void MapForCreating::feelObjSprites(int amount)
{
    int i;
    for (i = 0; i < amount; i++)
        addToObjSprites(i);
}


void MapForCreating::addToObjSprites(int number)
{
    switch(objects[number]->type)
    {
    case 0:
    {
        float w = objects[number]->width;
        float h = objects[number]->height;
        float sX = objects[number]->startX;
        float sY = objects[number]->startY;
        Button *button;
        button = new Button(startX + scale * sX * tilesize, startY + scale * sY * tilesize, 0, 0, 32, 32, w * scale * tilesize, h * scale * tilesize, playerPlace);
        objSprites.push_back(button);
        break;
    }

    case 1:
    {
        float w = objects[number]->width;
        float h = objects[number]->height;
        float sX = objects[number]->startX;
        float sY = objects[number]->startY;
        Button *button;
        button = new Button(startX + scale * sX * tilesize, startY + scale * sY * tilesize, 0, 0, 16 * w, 16 * h, w * scale * tilesize, h * scale * tilesize, platformPlace);
        objSprites.push_back(button);
        break;
    }
    }
}

void MapForCreating::correctPositions()
{
    clearObjSprites();
    scale = min(((float)windowWidth - spaceForButtons * 10) / tilesize / width, (float)windowHeight / tilesize / height);
    scale -= (scale * tilesize - (int)(scale * tilesize)) / tilesize;
    startX = spaceForButtons * 5 + (((float)windowWidth - spaceForButtons * 10) - tilesize * scale * width) / 2;
    startY = ((float)windowHeight - scale * tilesize * height) / 2;

    backSprite.setTextureRect(IntRect(0, 0, tilesize * scale * width, tilesize * scale * height));
    backSprite.setPosition(startX, startY);
    feelObjSprites(objects.size());
}

void MapForCreating::keyboardCommands()
{
    if (Keyboard::isKeyPressed(Keyboard::Up) && height < MaxMapH)
    {
        while(Keyboard::isKeyPressed(Keyboard::Up)){}
        height++;
        correctPositions();
    }


    if (Keyboard::isKeyPressed(Keyboard::Down) && height > 1)
    {
        while(Keyboard::isKeyPressed(Keyboard::Down)){}
        height--;
        correctPositions();
    }

    if (Keyboard::isKeyPressed(Keyboard::Right) && width < MaxMapW)
    {
        while(Keyboard::isKeyPressed(Keyboard::Right)){}
        width++;
        correctPositions();
    }


    if (Keyboard::isKeyPressed(Keyboard::Left) && width > 1)
    {
        while(Keyboard::isKeyPressed(Keyboard::Left)){}
        width--;
        correctPositions();
    }

    if (Mouse::isButtonPressed(Mouse::Left))
    {
        float mouseX = Mouse::getPosition().x;
        float mouseY = Mouse::getPosition().y;

        if (mouseX >= startX && mouseY >= startY)
        {
            mouseX -= startX;
            mouseY -= startY;
            tilesPhysics[(int)(mouseX / tilesize / scale)][(int)(mouseY / tilesize / scale)] = curTile + 1;
            tileUpdate((int)(mouseX / tilesize / scale), (int)(mouseY / tilesize / scale));
        }
    }

    if (Mouse::isButtonPressed(Mouse::Right))
    {
        float mouseX = Mouse::getPosition().x;
        float mouseY = Mouse::getPosition().y;

        if (mouseX >= startX && mouseY >= startY)
        {
            mouseX -= startX;
            mouseY -= startY;
            tilesPhysics[(int)(mouseX / tilesize / scale)][(int)(mouseY / tilesize / scale)] = 0;
            tileUpdate((int)(mouseX / tilesize / scale), (int)(mouseY / tilesize / scale));
            tiles[(int)(mouseX / tilesize / scale)][(int)(mouseY / tilesize / scale)] = 0;
        }
    }


}

void MapForCreating::tileUpdate(int x, int y)
{
    if (tilesPhysics[x][y] != 0)
    {
        int ID = countNeighbours(x, y);
        tiles[x][y] = ID + (tilesPhysics[x][y] - 1) * 16 + 1;
    }

    if (y > 0 && tilesPhysics[x][y - 1] != 0)
    {
        int IDup = countNeighbours(x, y - 1);
        tiles[x][y - 1] = IDup + (tilesPhysics[x][y - 1] - 1) * 16 + 1;
    }

    if (x < width - 1 && tilesPhysics[x + 1][y] != 0)
    {
        int IDright = countNeighbours(x + 1, y);
        tiles[x + 1][y] = IDright + (tilesPhysics[x + 1][y] - 1) * 16 + 1;
    }

    if (y < height - 1 && tilesPhysics[x][y + 1] != 0)
    {
        int IDdown = countNeighbours(x, y + 1);
        tiles[x][y + 1] = IDdown + (tilesPhysics[x][y + 1] - 1) * 16 + 1;
    }

    if (x > 0 && tilesPhysics[x - 1][y] != 0)
    {
        int IDleft = countNeighbours(x - 1, y);
        tiles[x - 1][y] = IDleft + (tilesPhysics[x - 1][y] - 1) * 16 + 1;
    }
}
int MapForCreating::countNeighbours(int x, int y)
{
    int friendAmount = underButtonsFriends[tilesPhysics[x][y] - 1].size();
    int i;
    int ID = 0;

    if (y == 0)
        ID += 1;
    if (x == width - 1)
        ID += 2;
    if (y == height - 1)
        ID += 4;
    if (x == 0)
        ID += 8;

    int oldID = ID;
    for (i = 0; i < friendAmount; i++)
        if (y > 0 && tilesPhysics[x][y - 1] != underButtonsFriends[tilesPhysics[x][y] - 1][i])
        {
            ID = oldID + 1;
        }
        else
        {
            ID = oldID;
            break;
        }

    oldID = ID;
    for (i = 0; i < friendAmount; i++)
        if (x < width - 1 && tilesPhysics[x + 1][y] != underButtonsFriends[tilesPhysics[x][y] - 1][i])
        {
            ID = oldID + 2;
        }
        else
        {
            ID = oldID;
            break;
        }

    oldID = ID;
    for (i = 0; i < friendAmount; i++)
        if (y < height - 1 && tilesPhysics[x][y + 1] != underButtonsFriends[tilesPhysics[x][y] - 1][i])
        {
            ID = oldID + 4;
        }
        else
        {
            ID = oldID;
            break;
        }

    oldID = ID;
    for (i = 0; i < friendAmount; i++)
        if (x > 0 && tilesPhysics[x - 1][y] != underButtonsFriends[tilesPhysics[x][y] - 1][i])
        {
            ID = oldID + 8;
        }
        else
        {
            ID = oldID;
            break;
        }

    return ID;
}

int MapForCreating::getNumber(ifstream &mapFile)
{
    int a = 0;
    char s = 10;
    while (s == 10 || s == 32)
    {
        mapFile.get(s);
        if (mapFile.eof())
            return -1;
    }
    while(s != 10 && s != 32)
    {
        a = a * 10 + s - '0';
        mapFile.get(s);
    }
    return a;
}

pair <int, int> MapForCreating::catchMouse()
{
    pair <int, int> positions;

    while(!Mouse::isButtonPressed(Mouse::Left)){}
        positions.first = (Mouse::getPosition().x - startX) / tilesize / scale;
        positions.second = (Mouse::getPosition().y - startY) / tilesize / scale;
    while(Mouse::isButtonPressed(Mouse::Left)){}
    return positions;
}

#include "MapForCreating.h"

 MapForCreating::MapForCreating(string tilesPlace, string backgroundPlace, string playerPlaceIn, string platformPlaceIn, string buttonPlaceIn, string boxPlaceIn)
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

    scale = min(((long double)windowWidth - spaceForButtons * 10) / tilesize / width, (long double)windowHeight / tilesize / height);

    startX = spaceForButtons * 5 + (((long double)windowWidth - spaceForButtons * 10) - tilesize * scale * width) / 2;
    startY = ((long double)windowHeight - scale * tilesize * height) / 2;
    scale -= (scale * tilesize - (int)(scale * tilesize)) / tilesize; //scale * tilesize need to be int

    Image backgroundImage;
    backgroundImage.loadFromFile(backgroundPlace);

    backTex.loadFromFile(backgroundPlace);
    backSprite.setTexture(backTex);
    backSprite.setTextureRect(IntRect(0, 0, tilesize * width, tilesize * height));
    backSprite.setScale(scale, scale);
    backSprite.setPosition(startX, startY);


    ifstream mapFile("settings//tilesButtonsInformation.txt");
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
            int tileX = getNumber(mapFile), tileY = getNumber(mapFile) + 15;
            Button *button = new Button(spaceAroundButtons, 100 + spaceForButtons * i, tileX * tilesize, tileY * tilesize, tilesize, tilesize, spaceForButtons - spaceAroundButtons * 2, spaceForButtons - spaceAroundButtons * 2, "images//Tile7.png");
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
    buttonPlace = buttonPlaceIn;
    boxPlace = boxPlaceIn;

    playerButton = new Button(windowWidth - spaceForObjButtons, 200, 0, 0, -1, -1, spaceForObjButtons - spaceAroundButtons * 2, spaceForObjButtons - spaceAroundButtons * 2, "images//PlayerButton.png");
    platformButton = new Button(windowWidth - spaceForObjButtons, 200 + spaceForObjButtons, 0, 0, -1, -1, spaceForObjButtons - spaceAroundButtons * 2, spaceForObjButtons - spaceAroundButtons * 2, "images//PlatformButton.png");
    buttonButton = new Button(windowWidth - spaceForObjButtons, 200 + spaceForObjButtons * 2, 0, 0, -1, -1, spaceForObjButtons - spaceAroundButtons * 2, spaceForObjButtons - spaceAroundButtons * 2, "images//ButtonButton.png");
    boxButton = new Button(windowWidth - spaceForObjButtons, 200 + spaceForObjButtons * 3, 0, 0, -1, -1, spaceForObjButtons - spaceAroundButtons * 2, spaceForObjButtons - spaceAroundButtons * 2, "images//BoxButton.png");
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

void MapForCreating::DrawMap(RenderWindow *window, long double time)
{

    int i, k;
    timeAfterPrevClick += time;
    (*window).draw(backSprite);
    if (process.first == 0)
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

    if (playerButton->buttonDisplayAndCheck(window, -1, -1) == 1)
    {
        process = {1, 0};
        inputObject = new ObjectInf(0);
        sizeChooseUI = new SizeChooseMenu();
        buttonsModeSet(0);
    }

    if (boxButton->buttonDisplayAndCheck(window, -1, -1) == 1)
    {
        process = {4, 0};
        inputObject = new ObjectInf(3);
        sizeChooseUI = new SizeChooseMenu();
        buttonsModeSet(0);
    }

    if (platformButton->buttonDisplayAndCheck(window, -1, -1) == 1)
    {
        process = {2, 0};
        inputObject = new ObjectInf(1);
        sizeChooseUI = new SizeChooseMenu();
        Image sliderBackImage;
        sliderBackImage.loadFromFile("images//SpeedSliderBack.png");
        long double sliderWidth = VideoMode::getDesktopMode().width / 3;
        long double sliderHeight = sliderWidth / 4;
        long double sliderStartX = VideoMode::getDesktopMode().width / 2 - sliderWidth / 2;
        long double sliderStartY = VideoMode::getDesktopMode().height / 2 - sliderHeight / 2;
        long double sliderScale = sliderWidth / sliderBackImage.getSize().x;
        speedSlider = new Slider(sliderStartX + sliderScale * 4, sliderStartY + sliderScale * 4, sliderScale * 5.5, sliderStartX + sliderWidth / 4 + sliderScale * 6, sliderStartY + sliderHeight / 2, sliderStartX + sliderWidth - sliderScale * 6, sliderStartY + sliderHeight / 2, 0, 5, sliderHeight / 3, 0.1, "images//Slider.png");


        sliderBackTex.loadFromImage(sliderBackImage);
        sliderBackSprite.setTexture(sliderBackTex);
        sliderBackSprite.setPosition(sliderStartX, sliderStartY);
        sliderBackSprite.setScale(sliderScale, sliderScale);

        okButton = new Button(sliderStartX + sliderWidth / 2 - 11 * sliderScale / 2, sliderStartY + sliderHeight - sliderScale * 9 / 2, 0, 0, 11, 9, 11 * sliderScale, 9 * sliderScale, "images//OKButton.png");
        buttonsModeSet(0);
    }

    if (buttonButton->buttonDisplayAndCheck(window, -1, -1) == 1)
    {
        process = {3, 4};
        inputObject = new ObjectInf(2);
        long double menuHeight = (long double)VideoMode::getDesktopMode().height / 3 * 2;
        long double menuWidth = menuHeight * 125 / 146;
        long double menuStartX = VideoMode::getDesktopMode().width / 2 - menuWidth / 2;
        long double menuStartY = VideoMode::getDesktopMode().height / 2 - menuHeight / 2;
        dynamicButtonCreatingUI = new DynamicButtonCreatingUI(menuStartX, menuStartY, menuWidth);
        buttonsModeSet(4); // and we wait for setting in displaySprites
    }

    displaySprites(window);

    if (process.first != 0)
    {
        processCheck(window);
    }


    Font font;
    font.loadFromFile("images//mainFont.ttf");
    Text infText("", font, 25);
    infText.setColor(Color::White);
    infText.setPosition(10 + windowWidth - spaceForObjButtons, 10);
    string s = to_string((int)width) + "    " + to_string((int)height) + "\n" + to_string((int)((Mouse::getPosition().x - startX) / tilesize / scale)) + "     " +  to_string((int)((Mouse::getPosition().y - startY) / tilesize / scale))
    + "\n\nT - try\nN - new\n";
    infText.setString(s);
    window->draw(infText);
}

void MapForCreating::processCheck(RenderWindow *window)
{
    switch (process.second)
    {
        case 0: // choose sizes
        {
            pair <int, int> sizeChooseUIReturn = sizeChooseUI->display(window);
            if (sizeChooseUIReturn.first == -1)
            {
                buttonsModeSet(1);
                process = {0, 0};
            }
            if (sizeChooseUIReturn.first > 0)
            {
                inputObject->width = sizeChooseUIReturn.first;
                inputObject->height = sizeChooseUIReturn.second;
                process.second++;
            }
            break;
        }
        case 1: // choose coordinates
        {
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                inputObject->startX = (int)((Mouse::getPosition().x - startX) / tilesize / scale);
                inputObject->startY = (int)((Mouse::getPosition().y - startY) / tilesize / scale);
                process.second++;
                if (process.first == 1 || process.first == 3 || process.first == 4) //player or button or box
                {
                    objects.push_back(inputObject);
                    addToObjSprites(objects.size() - 1);
                    buttonsModeSet(1);
                    process = {0, 0};
                }
                while(Mouse::isButtonPressed(Mouse::Left)){};
            }
            break;
        }
        case 2: // choose coordinates
        {
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                inputObject->endX = (int)((Mouse::getPosition().x - startX) / tilesize / scale);
                inputObject->endY = (int)((Mouse::getPosition().y - startY) / tilesize / scale);
                process.second++;
                while(Mouse::isButtonPressed(Mouse::Left)){};
            }
            break;
        }
        case 3: // choose speed
        {
            window->draw(sliderBackSprite);
            inputObject->speed = speedSlider->drawSliderGetValue(window);
            if (okButton->buttonDisplayAndCheck(window, -1, -1) == 1)
            {
                objects.push_back(inputObject), addToObjSprites(objects.size() - 1);
                buttonsModeSet(1);
                process = {0, 0};
            }
            break;
        }
        // case 4 is choose controlled platform for dynamicButton
        case 5:
        {
            int dynamicButtonCreatingUIReturn = dynamicButtonCreatingUI->draw(window);
            if (dynamicButtonCreatingUIReturn == -1)
            {
                buttonsModeSet(1);
                process = {0, 0};
            }
            if (dynamicButtonCreatingUIReturn == 1)
            {
                inputObject->mode = dynamicButtonCreatingUI->getValue('m');
                inputObject->off = dynamicButtonCreatingUI->getValue('f');
                inputObject->on = dynamicButtonCreatingUI->getValue('n');
                inputObject->dir = dynamicButtonCreatingUI->getValue('d');
                inputObject->width = dynamicButtonCreatingUI->getValue('l');
                process.second = 1;
            }
            break;
        }
    }
}

void MapForCreating::mapUpload (string mapFilePlace)
{
    ofstream mapFile(mapFilePlace);
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
        case 2:
        {
            mapFile << objects[i]->startX << " ";
            mapFile << objects[i]->startY << " ";
            mapFile << objects[i]->width << " ";
            mapFile << objects[i]->dir << " ";
            mapFile << objects[i]->controlled << " ";
            mapFile << objects[i]->mode << " ";
            mapFile << objects[i]->off << " ";
            mapFile << objects[i]->on << "\n";
            break;
        }
        case 3:
        {
            mapFile << objects[i]->width << " ";
            mapFile << objects[i]->height << " ";
            mapFile << objects[i]->startX << " ";
            mapFile << objects[i]->startY << "\n";
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
            long double w, h, sX, sY;
            w = getNumber(mapFile), h = getNumber(mapFile), sX = getNumber(mapFile), sY = getNumber(mapFile);
            ObjectInf *obj  = new ObjectInf(0); //0 - slime
            obj->width = w, obj->height = h;
            obj->startX = sX, obj->startY = sY;
            objects.push_back(obj);
            break;
        }
        case 1:
        {
            long double w, h, sX, sY, eX, eY, speed;
            w = getNumber(mapFile), h = getNumber(mapFile);
            sX = getNumber(mapFile), sY = getNumber(mapFile);
            eX = getNumber(mapFile), eY = getNumber(mapFile);
            speed = getNumber(mapFile);
            ObjectInf *obj  = new ObjectInf(1); //1 - platform
            obj->width = w, obj->height = h;
            obj->startX = sX, obj->startY = sY;
            obj->endX = eX, obj->endY = eY;
            obj->speed = speed;
            objects.push_back(obj);
            break;
        }
        case 2:
        {
            long double length, sX, sY, dir, controlObj, controlMode, onMode, offMode;
            sX = getNumber(mapFile), sY = getNumber(mapFile);
            length = getNumber(mapFile);
            dir = getNumber(mapFile);
            controlObj = getNumber(mapFile);
            controlMode = getNumber(mapFile);
            offMode = getNumber(mapFile), onMode = getNumber(mapFile);
            ObjectInf *obj = new ObjectInf(2);
            obj->startX = sX, obj->startY = sY;
            obj->controlled = controlObj;
            obj->mode = controlMode;
            obj->on = onMode, obj->off = offMode;
            obj->dir = dir, obj->width = length;
            objects.push_back(obj);
            break;
        }
        case 3:
        {
            long double w, h, sX, sY;
            w = getNumber(mapFile), h = getNumber(mapFile), sX = getNumber(mapFile), sY = getNumber(mapFile);
            ObjectInf *obj  = new ObjectInf(3); //3 - box
            obj->width = w, obj->height = h;
            obj->startX = sX, obj->startY = sY;
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
    int i, buttonReturn;
    for (i = 0; i < objSprites.size(); i++)
    {
        buttonReturn = objSprites[i]->buttonDisplayAndCheck(window, -1, -1);
        if (buttonReturn == 2)
        {
            vector <int> deleted;

            if (objects[i]->type == 1)
                deleted.push_back(i);
            else
            {
                delete objSprites[i];
                objSprites.erase(objSprites.begin() + i);
                delete objects[i];
                objects.erase(objects.begin() + i);
            }

            if (deleted.size() != 0)
            {
                for(int k = 0; k < objects.size(); k++)
                    if (objects[k]->type == 2)
                    {
                        if (objects[k]->controlled == i)
                        {
                            deleted.push_back(k);
                            continue;
                        }
                        int controlled = objects[k]->controlled;
                        for (int j = 0; j < deleted.size(); j++)
                            if (controlled > deleted[j])
                                objects[k]->controlled--;
                    }
                    sort(deleted.begin(), deleted.end());
                for (int j = 0; j < deleted.size(); j++)
                {
                    int del = deleted[j];
                    delete objSprites[del - j];
                    objSprites.erase(objSprites.begin() + del - j);
                    delete objects[del - j];
                    objects.erase(objects.begin() + del - j);
                }
            }
        }
        if (buttonReturn == 4 && objects[i]->type == 1) // choose controlled platform for dynamic button
        {
            inputObject->controlled = i;
            process.second++;
            buttonsModeSet(0);
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
        long double w = objects[number]->width;
        long double h = objects[number]->height;
        long double sX = objects[number]->startX;
        long double sY = objects[number]->startY;
        Button *button;
        button = new Button(startX + scale * sX * tilesize, startY + scale * sY * tilesize, 0, 0, 32, 32, w * scale * tilesize, h * scale * tilesize, playerPlace);
        objSprites.push_back(button);
        break;
    }

    case 1:
    {
        long double w = objects[number]->width;
        long double h = objects[number]->height;
        long double sX = objects[number]->startX;
        long double sY = objects[number]->startY;

        int texStartX = 0, texStartY = 0, i;
        for (i = 1; i < w; i++)
            texStartX += i * 16;
        for (i = 1; i < h; i++)
            texStartY += i * 16;
        Button *button;
        button = new Button(startX + scale * sX * tilesize, startY + scale * sY * tilesize, texStartX, texStartY, 16 * w, 16 * h, w * scale * tilesize, h * scale * tilesize, platformPlace);
        objSprites.push_back(button);
        break;
    }
    case 2:
    {
        long double w = objects[number]->width;
        long double h = objects[number]->height;
        long double sX = objects[number]->startX;
        long double sY = objects[number]->startY;

        int texStartX = 0, i;
        for (i = 1; i < w; i++)
            texStartX += i * 16;
        Button *button;
        switch(objects[number]->dir)
        {
        case 0:
            button = new Button(startX + scale * sX * tilesize, startY + scale * sY * tilesize - scale, texStartX, 0, 16 * w, 6, w * scale * tilesize, 6 * scale, buttonPlace);
            break;
        case 2:
            button = new Button(startX + scale * sX * tilesize, startY + scale * sY * tilesize + 10 * scale + scale, texStartX, 0, 16 * w, 6, w * scale * tilesize, 6 * scale, buttonPlace);
            break;
        case 3:
            button = new Button(startX + scale * sX * tilesize - scale, startY + scale * sY * tilesize, 0, texStartX + 6, 6, 16 * w, 6 * scale, w * scale * tilesize, buttonPlace);
            break;
        case 1:
            button = new Button(startX + scale * sX * tilesize + 10 * scale + scale, startY + scale * sY * tilesize, 0, texStartX + 6, 6, 16 * w, 6 * scale, w * scale * tilesize, buttonPlace);
            break;
        }
        objSprites.push_back(button);
        break;
    }
    case 3:
    {
        long double w = objects[number]->width;
        long double h = objects[number]->height;
        long double sX = objects[number]->startX;
        long double sY = objects[number]->startY;
        Button *button;
        int texStartX = 0, texStartY = 0, i;
        for (i = 1; i < w; i++)
            texStartX += i * 16;
        for (i = 1; i < h; i++)
            texStartY += i * 16;
        button = new Button(startX + scale * sX * tilesize, startY + scale * sY * tilesize, texStartX, texStartY, w * tilesize, h * tilesize, w * scale * tilesize, h * scale * tilesize, boxPlace);
        objSprites.push_back(button);
        break;
    }

    }
}

void MapForCreating::correctPositions()
{
    clearObjSprites();
    scale = min(((long double)windowWidth - spaceForButtons * 10) / tilesize / width, (long double)windowHeight / tilesize / height);
    scale -= (scale * tilesize - (int)(scale * tilesize)) / tilesize;
    startX = spaceForButtons * 5 + (((long double)windowWidth - spaceForButtons * 10) - tilesize * scale * width) / 2;
    startY = ((long double)windowHeight - scale * tilesize * height) / 2;

    backSprite.setTextureRect(IntRect(0, 0, tilesize * width, tilesize * height));
    backSprite.setScale(scale, scale);
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
        long double mouseX = Mouse::getPosition().x;
        long double mouseY = Mouse::getPosition().y;

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
        long double mouseX = Mouse::getPosition().x;
        long double mouseY = Mouse::getPosition().y;

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

long double MapForCreating::getNumber(ifstream &mapFile)
{
    int i;
    long double a = 0;
    int afterPoint = 0;
    char s = 10;
    while (s == 10 || s == 32)
        mapFile.get(s);
    while(s != 10 && s != 32)
    {
        if (s == '.')
            afterPoint = 1;
        else
            if (afterPoint == 0)
                a = a * 10 + s - '0';
            else
            {
                long double sNumber = s - '0';
                for (i = 0; i < afterPoint; i++)
                    sNumber /= 10;
                a += sNumber;
                afterPoint++;
            }
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

void MapForCreating::buttonsModeSet(int mode)
{
    int i;
    for (i = 0; i < objSprites.size(); i++)
        objSprites[i]->buttonModeSet(mode);
}

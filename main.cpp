#include "AllIncludes.h"


void darkWindow(RenderWindow *window, float gamma)
{
    RectangleShape square(Vector2f(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height));
    square.setPosition(0, 0);

    square.setFillColor(Color(0, 0, 0, gamma));
    window->draw(square);
}

class Darkness
{
public:
    Darkness(RenderWindow *windowIn, float lengthIn, int processIn)
    {
        window = windowIn;
        length = lengthIn;
        process = processIn;
    }
    int update(float time)
    {
        switch(process)
        {
        case -1:
            darkWindow(window, (length - timePass) / length * 255);
            timePass += time;
            if (timePass > length)
                return 0;
            return 1;
        case 1:
            darkWindow(window, (timePass) / length * 255);
            timePass += time;
            if (timePass > length)
                return 0;
            return 1;
        case 0:
            darkWindow(window, 255);
            timePass += time;
            if (timePass > length)
                return 0;
            return 1;

        }
    }
    RenderWindow *window;
    float length, timePass = 0;
    int process;
};



int main()
{
    Event event;
    int process = -10;
    int windowWidth = VideoMode::getDesktopMode().width, windowHeight = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(windowWidth, windowHeight), " " , Style::Fullscreen);

    MyMap *mainMap = NULL;
    MapForCreating *mainMapForCreating = NULL;
    LevelMenu *mainLevelMenu = NULL;
    MainMenu *mainMainMenu = NULL;
    LevelMap *mainLevelMap = NULL;
    int mainLevelMapWidth = 1000, mainLevelMapHeight = 1000;
    int mainLevelMapCurX, mainLevelMapCurY, mainLevelMapMaxY, mainLevelMapSpeed = 3;

    window.setFramerateLimit(200);
    Button mainButton(30, 30, 0, 0, 22, 9, 22 * 3, 9 * 3, "images//Butt.png");
    int lastLevel = 1;
    int levelChoose;
    Clock clock;
    float timePass = 0;
    float animationTimer = 0;
    int animationProcess = 0;
    int darknessProcessSet = 0, processPassed = 0;
    Darkness *darkness = NULL;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/1000;
        if (time > 50)
            time = 50;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Q))
                window.close();
        }

        switch(process)
        {
        case 0:   //Main Menu
        {
            window.clear();
            switch(mainMainMenu->drawAndCheckMenu(&window, time)) // 1-play 2-levels 3-create 0-nothing
            {
            case 1:
                process = -1, window.close();
                break;
            case 2:
                process = -4;
                break;
            case 3:
                process = -3;
                break;
            default:
                break;
            }
            break;
        }

        case 1: //play
        {
            //cout << "Start:\n";
            if (processPassed == 1)
            {
                if (darknessProcessSet == 0)
                {
                    processPassed = 0;
                    darknessProcessSet = 1;
                    process = 0;
                    darkness = new Darkness(&window, 2, -1);
                    delete mainMap;
                    break;
                }
                window.clear();
                mainMap->DrawMap(&window, time);
                break;
            }
            window.clear();
            if(mainMap->DrawMap(&window, time) == 2) // 2 - level passed
            {
                if (lastLevel != 0)
                    process = 5;
                else
                    process = 0;
            }

            if (mainButton.buttonDisplayAndCheck(&window,-1, -1) == 1)
            {
                darkness = new Darkness(&window, 2, 1);
                darknessProcessSet = 1;
                processPassed = 1;
                clock.restart();
            }

            if(Keyboard::isKeyPressed(Keyboard::R))
                process = -1;
            break;
        }

        case 2: //levels
        {
            window.clear();

            if (mainButton.buttonDisplayAndCheck(&window,-1, -1) == 1)
            {
                process = 0, delete mainLevelMenu, clock.restart();
                continue;
            }
            levelChoose = mainLevelMenu->drawAndCheckMenu(&window);
            if (levelChoose != 0)
                lastLevel = levelChoose, delete mainLevelMenu, process = -1;
            break;
        }

        case 3: //creating
        {
            timePass += time * 1000 * 1000000;
            window.clear(Color(66, 92, 110));
            mainMapForCreating->DrawMap(&window, time);
            if (Keyboard::isKeyPressed(Keyboard::U))
            {

                string way = "maps//";
                string s;
                cin >> s;
                way += s;
                way += ".txt";
                mainMapForCreating->mapUpload(way);
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {

                string way = "maps//";
                string s;
                cin >> s;
                way += s;
                way += ".txt";
                mainMapForCreating->mapDownload(way);
            }

            if (timePass > 3)
            {
                timePass = 0;
                mainMapForCreating->mapUpload("maps//level00.txt");
            }

            if (mainButton.buttonDisplayAndCheck(&window,-1, -1) == 1)
                process = 0, delete mainMapForCreating, clock.restart();
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::N))
            {
                delete mainMapForCreating;
                mainMapForCreating = new MapForCreating("images//Tile7.png", "images//Background.png", "images//Slime.png", "images//Platform.png", "images//DynamicButton.png", "images//HeavyBox.png");
            }

            if (Keyboard::isKeyPressed(Keyboard::T))
            {
                mainMap = new MyMap("maps//level00.txt", "images//Tile7.png", "images//Background.png");
                lastLevel = 0;
                process = 1;
                clock.restart();
            }

            break;
        }

        case 4:
        {
            window.clear();
            mainMainMenu->drawAndCheckMenu(&window, time, 1);

            if (mainLevelMapCurY > mainLevelMapMaxY)
            {
                mainLevelMapCurY -= mainLevelMapSpeed * time;
                if (mainLevelMapCurY < mainLevelMapMaxY)
                    mainLevelMapCurY = mainLevelMapMaxY;
                mainLevelMap->display(&window, time, mainLevelMapCurX, mainLevelMapCurY);
            }
            else
            {
                int level = mainLevelMap->display(&window, time);
                if (level != 0)
                    lastLevel = level, process = -1;

            }

            if (mainButton.buttonDisplayAndCheck(&window, -1, -1) == 1)
                process = 0, clock.restart();

            break;
        }


        case 5: //level was passed
        {
            window.clear();
            animationTimer += time / 500;
            if (animationTimer < 2)
            {
                switch(animationProcess)
                {
                case 0:
                    mainMap->DrawMap(&window, time);
                    darkWindow(&window, animationTimer / 2 * 255);
                    break;
                case 1:
                    darkWindow(&window, 255);
                    break;
                case 2:
                    mainMainMenu->drawAndCheckMenu(&window, time, 1);
                    mainLevelMap->display(&window, time);
                    darkWindow(&window, (2 - animationTimer) / 2 * 255);
                    break;

                }
            }
            else
                switch(animationProcess)
                {
                case 0:
                    delete mainMap;
                    animationTimer = 0;
                    animationProcess++;
                    break;
                case 1:
                    animationTimer = 0;
                    animationProcess++;
                    break;
                case 2:
                    animationTimer = 0;
                    animationProcess = 0;
                    mainLevelMap->startShowingNewLevel(lastLevel);
                    process = 4;
                    break;

                }
            break;

        }



        case -1: //download map
        {
            if (processPassed == -1)
            {
                if (darknessProcessSet == 0)
                {
                    process = 1, processPassed = 0;
                    darknessProcessSet = 1;
                    darkness = new Darkness(&window, 2, -1);
                }
                break;
            }

            string way = "maps//level";

            char s[3];
            s[0] = '0' + (lastLevel) / 10;
            s[1] = '0' + (lastLevel) % 10;
            s[2] = '\0';

            way += s;
            way += ".txt";

            mainMap = new MyMap(way, "images//Tile7.png", "images//Background.png");
            //process = 1;
            clock.restart();

            processPassed = -1;
            darkness = new Darkness(&window, 2, 1);
            darknessProcessSet = 1;
            break;
        }

        case -2://download
        {
            mainLevelMenu = new LevelMenu;
            process = 2;
            break;
        }

        case -4:// download LevelMap
        {
            if (mainLevelMap == NULL)
                mainLevelMap = new LevelMap(mainLevelMapWidth);
            process = 4;
            mainLevelMapCurY = VideoMode::getDesktopMode().height;
            mainLevelMapMaxY = (VideoMode::getDesktopMode().height - mainLevelMapHeight) / 2;
            mainLevelMapCurX = (VideoMode::getDesktopMode().width - mainLevelMapWidth) / 2;
            clock.restart();
            break;
        }

        case -3://download
        {
            mainMapForCreating = new MapForCreating("images//Tile7.png", "images//Background.png", "images//Slime.png", "images//Platform.png", "images//DynamicButton.png", "images//HeavyBox.png");
            mainMapForCreating->mapDownload("maps//level00.txt");
            process = 3;
            clock.restart();
            break;
        }

        case -10: // -0 download
        {
            mainMainMenu = new MainMenu;
            process = 0;
            clock.restart();
            break;
        }

        }


        if (darknessProcessSet)
        {
            if(!(darkness->update(time / 500)))
                darknessProcessSet = 0, delete darkness;
        }
        window.display();
    }
    return 0;
}

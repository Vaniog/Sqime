#include "AllIncludes.h"




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
    AllHitboxInf *AHI = NULL;

    window.setFramerateLimit(1000);
    Button mainButton(30, 30, 0, 0, 22, 9, 22 * 3, 9 * 3, "images\\Butt.png");
    int lastLevel = 1;
    int levelChoose;
    Clock clock;
    float timePass = 0;
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/1000;
        if (time > 50)
            time = 50;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
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
                process = -1, delete mainMainMenu;
                break;
            case 2:
                process = -2, delete mainMainMenu;
                break;
            case 3:
                process = -3, delete mainMainMenu;
                break;
            }
            window.display();
            break;
        }

        case 1: //play
        {
            //cout << "Start:\n";
            window.clear();
            if(mainMap->DrawMap(&window, time) == 2) // 2 - level passed
                process = -10;
            if (mainButton.buttonDisplayAndCheck(&window,-1, -1) == 1)
                process = -10, delete mainMap;
            if(Keyboard::isKeyPressed(Keyboard::R))
                process = -1;
            window.display();
            break;
        }

        case 2: //levels
        {
            window.clear();

            if (mainButton.buttonDisplayAndCheck(&window,-1, -1) == 1)
            {
                process = -10, delete mainLevelMenu;
                continue;
            }
            levelChoose = mainLevelMenu->drawAndCheckMenu(&window);
            if (levelChoose != 0)
                lastLevel = levelChoose, delete mainLevelMenu, process = -1;
            window.display();
            break;
        }

        case 3: //creating
        {
            timePass += time * 1000 * 1000000;
            window.clear(Color(66, 92, 110));
            mainMapForCreating->DrawMap(&window, time);
            if (Keyboard::isKeyPressed(Keyboard::U))
            {

                string way = "maps\\\\";
                string s;
                cin >> s;
                if(s[0] == '\\')
                    continue;

                way += s;
                way += ".txt";
                mainMapForCreating->mapUpload(way);
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {

                string way = "maps\\\\";
                string s;
                cin >> s;
                if(s[0] == '\\')
                    continue;
                way += s;
                way += ".txt";
                mainMapForCreating->mapDownload(way);
            }

            if (timePass > 3)
            {
                timePass = 0;
                mainMapForCreating->mapUpload("maps\\level00.txt");
            }

            if (mainButton.buttonDisplayAndCheck(&window,-1, -1) == 1)
                process = -10, delete mainMapForCreating;
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::N))
            {
                delete mainMapForCreating;
                mainMapForCreating = new MapForCreating("images\\Tile7.png", "images\\Background.png", "images\\Slime.png", "images\\Platform.png");
            }

            if (Keyboard::isKeyPressed(Keyboard::T))
            {
                mainMap = new MyMap("maps\\level00.txt", "images\\Tile7.png", "images\\Background.png");
                lastLevel = 0;
                process = 1;
                clock.restart();
            }

            break;
        }


        case -1: //download map and player
        {
            string way = "maps\\\\level";

            char s[3];
            s[0] = '0' + (lastLevel) / 10;
            s[1] = '0' + (lastLevel) % 10;
            s[2] = '\0';

            way += s;
            way += ".txt";

            mainMap = new MyMap(way, "images\\Tile7.png", "images\\Background.png");
            process = 1;
            clock.restart();
            break;
        }

        case -2://download
        {
            mainLevelMenu = new LevelMenu;
            process = 2;
            clock.restart();
            break;
        }

        case -3://download
        {
            mainMapForCreating = new MapForCreating("images\\Tile7.png", "images\\Background.png", "images\\Slime.png", "images\\Platform.png");
            mainMapForCreating->mapDownload("maps\\level00.txt");
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
    }
    return 0;
}

cmake_minimum_required(VERSION 3.0)

project(sqime)

include_directories(include)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(main main.cpp
        include/AllIncludes.h
        include/DynamicObject.h src/DynamicObject.cpp
        include/AllHitboxInf.h src/AllHitboxInf.cpp
        include/Button.h src/Button.cpp
        include/LevelMenu.h src/LevelMenu.cpp
        include/MainMenu.h src/MainMenu.cpp
        include/MapForCreating.h src/MapForCreating.cpp
        include/MyMap.h src/MyMap.cpp
        include/Platform.h src/Platform.cpp
        include/Player.h src/Player.cpp
        include/Box.h src/Box.cpp
        include/DynamicButton.h src/DynamicButton.cpp
        include/DynamicButtonCreatingUI.h src/DynamicButtonCreatingUI.cpp
        include/SizeChooseMenu.h src/SizeChooseMenu.cpp
        include/Slider.h src/Slider.cpp
        include/LevelMap.h src/LevelMap.cpp)

target_link_libraries(main sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
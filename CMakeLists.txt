cmake_minimum_required(VERSION 3.0)

project(sqime)

include_directories(include)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_library(DynamicObject STATIC include/DynamicObject.h DynamicObject.cpp)

add_library(AllHitboxInf STATIC include/AllHitboxInf.h AllHitboxInf.cpp)

add_library(Button STATIC include/Button.h Button.cpp)

add_library(LevelMenu STATIC include/LevelMenu.h LevelMenu.cpp)

add_library(MainMenu STATIC include/MainMenu.h MainMenu.cpp)

add_library(MapForCreating STATIC include/MapForCreating.h MapForCreating.cpp)

add_library(MyMap STATIC include/MyMap.h MyMap.cpp)

add_library(Platform STATIC include/Platform.h Platform.cpp)

add_library(Player STATIC include/Player.h Player.cpp)

add_library(DynamicButton STATIC include/DynamicButton.h DynamicButton.cpp)

add_library(DynamicButtonCreatingUI STATIC include/DynamicButtonCreatingUI.h DynamicButtonCreatingUI.cpp)

add_library(Box STATIC include/Box.h Box.cpp)

add_library(SizeChooseMenu STATIC include/SizeChooseMenu.h SizeChooseMenu.cpp)

add_library(Slider STATIC include/Slider.h Slider.cpp)

add_library(LevelMap STATIC include/LevelMap.h LevelMap.cpp)

add_executable(main main.cpp include/AllIncludes.h DynamicObject AllHitboxInf Button LevelMenu MainMenu MapForCreating MyMap Platform Player Box DynamicButton
        DynamicButtonCreatingUI SizeChooseMenu Slider LevelMap)

target_link_libraries(main sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
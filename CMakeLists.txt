cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
project(Sqime)

include_directories(include)

if (WIN32)
    set(SFML_DIR "C:\\SFML\\lib\\cmake\\SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
endif (WIN32)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Sqime main.cpp
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

target_link_libraries(Sqime sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
# Sqime

## This is a game ##
---
![pic](images/ForREADME/img.png) I'm Vaniog, my discord: **Vaniog#1886** \
This readme and release for my training

---
## Table of Contents:
- [Description](#description)
- [Quick setup](#quick-setup)
- [Playing](#playing)
- [Building](#building)
---

## Description

This is my old project, so here it is,
bad code (watch good [issues](https://github.com/Vaniog/Sqime/issues) from [bogdasar](https://github.com/bogdasar1985)), bad file organization and much more.
But the game is not so bad.
I cleaned as much as I can, I added README, CMakeLists, and I deleted trash from repository.
If sometime I'll want to return to this game, I'll write it from scratch.

---
## Quick setup

For quick setup just install project from the [latest release](https://github.com/Vaniog/Sqime/releases/tag/v1.0). \
It has Windows, Linux, MacOS versions. \
If you want to build project yourself, watch [building](#building)

---

## Playing

### In game:

> moving - **w, a, s, d** \
> restart level - **r**

### In creating mode

Your created level will be saved at maps/level00.txt, share it!
> #### <u>Main</u>
>- new field - **n** 
>- try level - **t** 
>- field size change - **arrows up and down** 
>- to choose tile to draw - **buttons on left sight** 
>- to change tile rotation - **double-click on tile button**

> #### How to make <u>slime</u>
>- Push slime button 
>- Choose size 
>- Choose coords (you can see coords in top-right corner)

> #### How to make <u>platforms</u>:
>- Push platform button 
>- Choose size 
>- Choose starting and ending coords 
>- Choose speed

> #### How to make <u>box</u>
>- Look at [how to make slime](#how-to-make-slime)

> #### How to make <u>buttons</u>
> <i>Carefully! code is bad, create buttons after other objects</i> 
>- Push button button 
>- Choose platform to control 
>- Choose where will platform go on off-mode on-mode 
>- Choose mode of button (just experiment with it) 
>- Choose size, and when choose coords
 ---
 
## Building

This project is running Simple Fast Multimedia Library
(**SFML**) \
SFML: [Github](https://github.com/SFML/SFML) [Website](https://www.sfml-dev.org/)

- [Linux](#linux)
- [MacOS](#macos)
- [Windows](#windows)

### Linux
    #installing cmake
    sudo apt-get install cmake

    #installing sfml library
    sudo apt-get install libsfml-dev

    #clone github project
    git clone https://github.com/Vaniog/Sqime
    cd Sqime

    #building project
    mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    
    #and launch
    cd ..
    build/Sqime

### MacOS
    #if you haven't brew, install it
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    
    #inctalling cmake
    brew install cmake

    #installing sfml library
    brew install sfml

    #clone github project
    git clone https://github.com/Vaniog/Sqime
    cd Sqime

    #building project
    mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    
    #and launch
    cd ..
    build/Sqime

### Windows
To build with **CMake** you need to have **SFML** at **C:/**, (C:/SFML), or in **PATH**. 
Select **"Sqime/"** as launch directory (launch **.exe** from **"/Sqime"**).

I cant give just code for terminal to build is easily on **Windows**.
Maybe your **IDE** running with **CMake** projects (**CLion, VSCode** with plugins).

You still can download built project from the [latest release](https://github.com/Vaniog/Sqime/releases/tag/v1.0)

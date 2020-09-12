# engine99
[![Build Status](https://travis-ci.com/AndrewChe7/engine99.svg?token=A3tSJzqqxN8pUfRxxzsu&branch=master)](https://travis-ci.com/AndrewChe7/engine99) 
[![CMake](https://github.com/AndrewChe7/engine99/workflows/CMake/badge.svg)](https://github.com/AndrewChe7/engine99/actions)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/2a8c9251852f4c4f856aaaabd5ae895d)](https://www.codacy.com?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=AndrewChe7/engine99&amp;utm_campaign=Badge_Grade)

It's gonna be really cool game engine made with C99 technologies. Only open source, only hardcore.
I use SDL for cross platform input and window drawing, opengl for rendering, nuklear for nice
imgui, flecs as ECS framework.

## Build

1. Install dependencies: 
    * [SDL2](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php)
    * [GLEW](http://glew.sourceforge.net/install.html)
    * [cglm](https://github.com/recp/cglm)

2. `mkdir build && cd build`

3. `cmake ..` \
    By default this line build game for `game` directory, but you can specify your game with `cmake .. -DGAME_DIR=custom_game`

4. `make`

## API

TODO

## Features, I'd like to implement
* Main features (rendering, input, physics, sound system and other)
* Scripting language (I really love python)
* ECS
* Modules and plugins system
* Developer tools and other stuff

## Requirements for contributors
* Only open source third party tools
* Only C99 compatible code
* Cross platform compatibility

# engine99
[![Build Status](https://travis-ci.com/AndrewChe7/engine99.svg?token=A3tSJzqqxN8pUfRxxzsu&branch=master)](https://travis-ci.com/AndrewChe7/engine99)

It's gonna be really cool game engine made with C99 technologies. Only open source, only hardcore.
I use SDL for cross platform input and window drawing, opengl for rendering, nuklear for nice
imgui, flecs as ECS framework.

## Build

1. Install dependencies: [SDL2](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php) and [GLEW](http://glew.sourceforge.net/install.html).
1. `mkdir build && cd build`
1. `cmake ..` \
    By default this line build game for `game` directory, but you can specify your game with `cmake .. -DGAME_DIR=custom_game`
1. `make`

## API

TODO

## Features, I'd like to implement:
* Main features (rendering, input, physics, sound system and other)
* Scripting language (I really love python)
* ECS
* Modules and plugins system
* Developer tools and other stuff

## Requirements for contributors:
* Only open source third party tools
* Only C99 compatible code
* Cross platform compatibility

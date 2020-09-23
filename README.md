# engine99

[![Build Status](https://travis-ci.com/engine99team/engine99.svg?branch=master)](https://travis-ci.com/engine99team/engine99)
[![CMake](https://github.com/engine99team/engine99/workflows/CMake/badge.svg)](https://github.com/engine99team/engine99/actions)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/72bf37a997b5448cbff0767dd94dbb7d)](https://www.codacy.com/gh/engine99team/engine99/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=engine99team/engine99&amp;utm_campaign=Badge_Grade)
[![Build status](https://ci.appveyor.com/api/projects/status/ifw6fp4sto7s7cr7?svg=true)](https://ci.appveyor.com/project/AndrewChe7/engine99)
[![Docs](https://img.shields.io/badge/docs-docsforge-blue)](https://engine99.docsforge.com/)

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

* [ ] Main features 
  * [ ] rendering 
    * [x] load textures
    * [x] load shaders
    * [x] load 3d models
    * [x] render simple shapes
    * [x] render 3d models
    * [ ] normals
    * [ ] lightning
      * [ ] point light
      * [ ] directional light
      * [ ] spot light
      * [ ] lightmaps
      * [ ] shadows
    * [ ] PBR shading
  * [x] input
  * [ ] physics 
  * [ ] sound system 
  * [ ] UI
    * [ ] Simple GUI
    * [x] imgui for tools
  * [ ] other
* [ ] Scripting language (I really love python)
* [x] ECS
* [x] Modules and plugins system
* [ ] Developer tools and other stuff

## Requirements for contributors

* Only open source third party tools
* Only C99 compatible code
* Cross platform compatibility

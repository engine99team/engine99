#ifndef ENGINE99_GLOBALS_H
#define ENGINE99_GLOBALS_H
#include <flecs.h>
#include <nuklear_include.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//ECS
extern ecs_world_t* world;
ECS_TAG_EXTERN(global_tag);

//Render
extern SDL_GLContext gl_context;

//Window
extern bool quit_flag;
extern SDL_Event event;
extern SDL_Window* window;
extern int window_width;
extern int window_height;
extern struct nk_colorf bg;

//imgui
extern struct nk_context *nk_ctx;
extern struct nk_font_atlas *atlas;


#endif //ENGINE99_GLOBALS_H

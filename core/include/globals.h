#ifndef ENGINE99_GLOBALS_H
#define ENGINE99_GLOBALS_H
#include <flecs.h>
#include <nuklear_include.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <spng.h>

/** Global ECS world */
extern ecs_world_t* world;

/** Global tag for rendering windows, running singleton systems etc */
extern ecs_entity_t global_tag;

ECS_TAG_EXTERN(global_tag);

/** global opengl context */
extern SDL_GLContext gl_context;

/** Main loop ends, when quit_flag is true */
extern bool quit_flag;

/** One event to rule the world... */
extern SDL_Event event;

/** Global SDL window */
extern SDL_Window* window;
extern int window_width;
extern int window_height;

/** Window background */
extern struct nk_colorf bg;

/** imgui context */
extern struct nk_context *nk_ctx;

/** imgui fonts */
extern struct nk_font_atlas *atlas;

#endif //ENGINE99_GLOBALS_H

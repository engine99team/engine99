#include <flecs.h>
#include <nuklear_include.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <spng.h>

ecs_world_t* world;
bool quit_flag;
ECS_TAG_DECLARE(global_tag);

SDL_Window* window;
int window_width, window_height;
SDL_Event event;
SDL_GLContext gl_context;
struct nk_colorf bg = {0.10f, 0.18f, 0.24f, 1.0f};

struct nk_context *nk_ctx;
struct nk_font_atlas *atlas;

#include "window.h"
#include <log.h>
#include <nuklear_include.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "globals.h"
#include "config.h"


int create_window() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_error("Failed to init SDL");
        return 1;
    }

    window = SDL_CreateWindow(CONFIG_WINDOW_TITLE, 0, 0,
                              CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    if (window == NULL) {
        log_error("Failed to create window");
        return 1;
    }

    gl_context = SDL_GL_CreateContext(window);

    if (glewInit() != GLEW_OK) {
        log_error("Failed to setup GLEW\n");
        return 1;
    }

    return 0;
}

int destroy_window() {
    SDL_GL_DeleteContext(gl_context);
    if (window != NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void pre_render_window_system (ecs_iter_t* it) {
    SDL_GetWindowSize(window, &window_width, &window_height);
    glViewport(0, 0, window_width, window_height);
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void post_render_window_system (ecs_iter_t* it) {
    nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
    SDL_GL_SwapWindow(window);
}
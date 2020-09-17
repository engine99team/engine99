#include "window.h"
#include <log.h>
#include <nuklear_include.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "globals.h"
#include "config.h"

/**
 * @brief Creates window with SDL2 and init opengl context in it
 * @return zero if there's no problems
 */
int create_window() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_error("Failed to init SDL");
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    window = SDL_CreateWindow(CONFIG_WINDOW_TITLE, 0, 0,
                              CONFIG_WINDOW_WIDTH, CONFIG_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    if (window == NULL) {
        log_error("Failed to create window. %s", SDL_GetError());
        return 1;
    }
    glEnable(GL_MULTISAMPLE);
    gl_context = SDL_GL_CreateContext(window);
#ifndef __APPLE__
    if (glewInit() != GLEW_OK) {
        log_error("Failed to setup GLEW\n");
        return 1;
    }
#endif
    return 0;
}

/**
 * @brief destroys window and frees memory
 */
int destroy_window() {
    SDL_GL_DeleteContext(gl_context);
    if (window != NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void pre_render_window_system (ecs_iter_t* it) {
    SDL_GetWindowSize(window, &window_width, &window_height);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window_width, window_height);
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void post_render_window_system (ecs_iter_t* it) {
    nk_sdl_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
    glEnable(GL_DEPTH_TEST);
    SDL_GL_SwapWindow(window);
}

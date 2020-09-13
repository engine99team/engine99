#include <nuklear_include.h>
#include <SDL2/SDL.h>
#include "events.h"
#include "globals.h"
#include "game.h"

void process_events_system (ecs_iter_t* it) {
    nk_input_begin(nk_ctx);
    while (SDL_PollEvent(&event) != 0)
    {
        nk_sdl_handle_event(&event);
        game_event(&event, it->delta_time);
        if (event.type == SDL_QUIT)
        {
            quit_flag = true;
        }
    }
    nk_input_end(nk_ctx);
}

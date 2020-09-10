#include <nuklear_include.h>
#include "globals.h"
#include "stages.h"
#include "example.h"

int init_game (void) {
    init_example();
    return 0;
}

int game_event (SDL_Event* event) {
    return 0;
}

int destroy_game (void) {
    destroy_example();
    return 0;
}

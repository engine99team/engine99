#ifndef ENGINE99_EXAMPLE_H
#define ENGINE99_EXAMPLE_H
#include <SDL.h>

int init_example(void);
int example_events (SDL_Event* event, float delta_time);
int destroy_example(void);

#endif //ENGINE99_EXAMPLE_H

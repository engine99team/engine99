#ifndef ENGINE99_WINDOW_H
#define ENGINE99_WINDOW_H
#include <flecs.h>

int create_window();

int destroy_window();

void pre_render_window_system (ecs_iter_t* it);

void post_render_window_system (ecs_iter_t* it);

#endif //ENGINE99_WINDOW_H

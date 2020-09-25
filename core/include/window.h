#ifndef ENGINE99_WINDOW_H
#define ENGINE99_WINDOW_H
#include <flecs.h>

/**
 * Creates window with SDL2 and init opengl context in it
 * @return zero if there's no problems
 */
int create_window();

/**
 * Destroys window and frees memory
 */
int destroy_window();

void pre_render_window_system (ecs_iter_t* it);

void post_render_window_system (ecs_iter_t* it);

#endif //ENGINE99_WINDOW_H

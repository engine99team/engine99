#include <stdio.h>

#include <log.h>
#include <flecs.h>
#include <nuklear_include.h>
#include <graphics_components.h>

#include "window.h"
#include "events.h"
#include "globals.h"
#include "light.h"
#include "game.h"
#include "stages.h"
#include "core_components.h"
#include "simple_meshes.h"
#include "config.h"
#include "models.h"

int main() {
    int res;
    world = ecs_init();
    if (world == NULL) {
        log_fatal("Can't init flecs world");
        return -1;
    }
    res = create_window();
    if (res != 0) {
        log_fatal("Can't create SDL window");
        ecs_fini(world);
        return -1;
    }
    nk_ctx = nk_sdl_init(window);
    if (nk_ctx == NULL) {
        log_fatal("Can't init nuklear context");
        destroy_window();
        ecs_fini(world);
        return -1;
    }
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
    init_stages();
    init_core_components();
    init_graphics_components();
    init_simple_meshes();
    init_models();
    init_lights();
    init_graphics();
    ECS_TAG_DEFINE(world, global_tag);
    ECS_ENTITY(world, global_entity, global_tag);

    ECS_SYSTEM(world, pre_render_window_system, pre_render_stage, global_tag);
    ECS_SYSTEM(world, post_render_window_system, post_render_stage, global_tag);
    ECS_SYSTEM(world, process_events_system, events_stage, global_tag);

    res = init_game();
#ifdef CONFIG_FPS
    ecs_set_target_fps(world, CONFIG_FPS);
#endif
    if (res != 0) {
        nk_sdl_shutdown();
        destroy_window();
        ecs_fini(world);
        log_fatal("Can't init game");
        return -1;
    }

    while (!quit_flag) {
        ecs_progress(world, 0);
    }
    nk_sdl_shutdown();
    destroy_window();
    ecs_fini(world);
    return 0;
}

#include <stdio.h>

#include <log.h>
#include <flecs.h>
#include <nuklear_include.h>

#include "window.h"
#include "events.h"
#include "globals.h"
#include "game.h"
#include "stages.h"

int main() {
    world = ecs_init();
    create_window();
    nk_ctx = nk_sdl_init(window);
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
    init_stages();
    ECS_TAG_DEFINE(world, global_tag);
    ECS_ENTITY(world, global_entity, global_tag);

    ECS_SYSTEM(world, pre_render_window_system, pre_render_stage, global_tag);
    ECS_SYSTEM(world, post_render_window_system, post_render_stage, global_tag);
    ECS_SYSTEM(world, process_events_system, events_stage, global_tag);

    init_game();

    while (!quit_flag) {
        ecs_progress(world, 0);
    }

    nk_sdl_shutdown();
    destroy_window();
    ecs_fini(world);
    return 0;
}

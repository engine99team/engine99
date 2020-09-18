#include "globals.h"
#include <flecs.h>

// Clears window and set depth buffer
ECS_TAG_DECLARE(pre_render_stage);
// Listens player input
ECS_TAG_DECLARE(events_stage);
// Game logic
ECS_TAG_DECLARE(update_stage);
// Render objects
ECS_TAG_DECLARE(render_stage);
// Render GUI layer
ECS_TAG_DECLARE(gui_stage);
// Render imgui layer
ECS_TAG_DECLARE(imgui_stage);
// End of frame
ECS_TAG_DECLARE(post_render_stage);

/**
 * @brief inits stages of pre- post- and render
 * @return zero if no problems
 */
int init_stages (void) {
    ECS_TAG_DEFINE(world, events_stage);
    ECS_TAG_DEFINE(world, update_stage);
    ECS_TAG_DEFINE(world, pre_render_stage);
    ECS_TAG_DEFINE(world, render_stage);
    ECS_TAG_DEFINE(world, gui_stage);
    ECS_TAG_DEFINE(world, imgui_stage);
    ECS_TAG_DEFINE(world, post_render_stage);
    ECS_PIPELINE(world, pipeline, events_stage, update_stage, \
                 gui_stage, imgui_stage, \
                 pre_render_stage, render_stage, post_render_stage)
    ecs_set_pipeline(world, pipeline);
    return 0;
}

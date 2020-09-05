#include "globals.h"
#include <flecs.h>

ecs_entity_t pre_render_stage;
ecs_entity_t events_stage;
ecs_entity_t update_stage;
ecs_entity_t render_stage;
ecs_entity_t gui_stage;
ecs_entity_t imgui_stage;
ecs_entity_t post_render_stage;
ecs_entity_t global_tag;

int init_stages (void) {
    events_stage = ecs_set(world, 0, EcsName, {"events_stage"});
    update_stage = ecs_set(world, 0, EcsName, {"update_stage"});
    pre_render_stage = ecs_set(world, 0, EcsName, {"pre_render_stage"});
    render_stage = ecs_set(world, 0, EcsName, {"render_stage"});
    gui_stage = ecs_set(world, 0, EcsName, {"gui_stage"});
    imgui_stage = ecs_set(world, 0, EcsName, {"imgui_stage"});
    post_render_stage = ecs_set(world, 0, EcsName, {"post_render_stage"});
    global_tag = ecs_set(world, 0, EcsName, {"global_tag"});
    ECS_PIPELINE(world, pipeline, events_stage, update_stage, \
                 gui_stage, imgui_stage, \
                 pre_render_stage, render_stage, post_render_stage)
    ecs_set_pipeline(world, pipeline);
    return 0;
}
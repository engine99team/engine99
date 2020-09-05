#ifndef ENGINE99_STAGES_H
#define ENGINE99_STAGES_H
#include <flecs.h>

extern ecs_entity_t pre_render_stage;
extern ecs_entity_t events_stage;
extern ecs_entity_t update_stage;
extern ecs_entity_t render_stage;
extern ecs_entity_t gui_stage;
extern ecs_entity_t imgui_stage;
extern ecs_entity_t post_render_stage;
extern ecs_entity_t global_tag;

int init_stages (void);

#endif //ENGINE99_STAGES_H

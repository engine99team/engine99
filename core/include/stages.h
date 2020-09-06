#ifndef ENGINE99_STAGES_H
#define ENGINE99_STAGES_H
#include <flecs.h>

ECS_TAG_EXTERN(pre_render_stage);
ECS_TAG_EXTERN(events_stage);
ECS_TAG_EXTERN(update_stage);
ECS_TAG_EXTERN(render_stage);
ECS_TAG_EXTERN(gui_stage);
ECS_TAG_EXTERN(imgui_stage);
ECS_TAG_EXTERN(post_render_stage);

int init_stages (void);

#endif //ENGINE99_STAGES_H

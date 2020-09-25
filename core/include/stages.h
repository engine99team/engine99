#ifndef ENGINE99_STAGES_H
#define ENGINE99_STAGES_H
#include <flecs.h>

//Just for doxygen

/**
 * Stage, that runs at the start of the frame and clears window
 */
extern ecs_entity_t pre_render_stage;

/**
 * Stage, that is used to get player input
 */
extern ecs_entity_t events_stage;

/**
 * Stage, that is used to update game logic components
 */
extern ecs_entity_t update_stage;

/**
 * Stage, that used to render objects in game world
 */
extern ecs_entity_t render_stage;

/**
 * Stage, that used to render GUI
 */
extern ecs_entity_t gui_stage;

/**
 * Stage, that used to render imgui
 */
extern ecs_entity_t imgui_stage;

/**
 * Stage, that ends frame
 */
extern ecs_entity_t post_render_stage;


ECS_TAG_EXTERN(pre_render_stage);
ECS_TAG_EXTERN(events_stage);
ECS_TAG_EXTERN(update_stage);
ECS_TAG_EXTERN(render_stage);
ECS_TAG_EXTERN(gui_stage);
ECS_TAG_EXTERN(imgui_stage);
ECS_TAG_EXTERN(post_render_stage);

/**
 * @brief inits stages of pre- post- and render
 * @return zero if no problems
 */
int init_stages (void);

#endif //ENGINE99_STAGES_H

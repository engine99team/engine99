#include <flecs.h>
#include "graphics_components.h"

ECS_COMPONENT_DECLARE(Texture);
ECS_COMPONENT_DECLARE(ShaderProgram);
ECS_COMPONENT_DECLARE(Camera);

void delete_texture(ecs_iter_t* it) {
    Texture* texture = ecs_column(it, Texture, 1);
    glDeleteTextures(1, &texture->id);
}

void delete_shader_program(ecs_iter_t* it) {
    ShaderProgram* shader_program = ecs_column(it, ShaderProgram, 1);
    glDeleteProgram(shader_program->id);
}

/**
 * @brief Inits graphics components
 * @return zero if no problems
 */
int init_graphics_components(void) {
    ECS_COMPONENT_DEFINE(world, Texture);
    ECS_COMPONENT_DEFINE(world, ShaderProgram);
    ECS_COMPONENT_DEFINE(world, Camera);
    ECS_TRIGGER(world, delete_texture, EcsOnRemove, Texture);
    ECS_TRIGGER(world, delete_shader_program, EcsOnRemove, ShaderProgram);
    ECS_ENTITY(world, cameraEntity, Camera, Transform);
    ecs_set(world, cameraEntity, Camera, {
        .fov=GLM_PI_2f,
        .near=0.1f,
        .far=100.f
    });
    ecs_set(world, cameraEntity, Transform, {
        .position = {0, 0, 0},
        .rotation = {0, 0, 0}
    });
}

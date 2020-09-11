#include <flecs.h>
#include "graphics_components.h"

ECS_DECLARE_COMPONENT(Texture);
ECS_DECLARE_COMPONENT(ShaderProgram);

void delete_texture(ecs_iter_t* it) {
    Texture* texture = ecs_column(it, Texture, 1);
    glDeleteTextures(1, &texture->id);
}

void delete_shader_program(ecs_iter_t* it) {
    ShaderProgram* shader_program = ecs_column(it, ShaderProgram, 1);
    glDeleteProgram(shader_program->id);
}

int init_graphics_components(void) {
    ECS_COMPONENT_DEFINE(world, Texture);
    ECS_COMPONENT_DEFINE(world, ShaderProgram);
    ECS_TRIGGER(world, delete_texture, EcsOnRemove, Texture);
    ECS_TRIGGER(world, delete_shader_program, EcsOnRemove, ShaderProgram);

}

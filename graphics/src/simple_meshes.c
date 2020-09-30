#include "simple_meshes.h"
#include "globals.h"
#include "stages.h"

ECS_COMPONENT_DECLARE(TriangleMesh);
ECS_COMPONENT_DECLARE(RectangleMesh);
ECS_COMPONENT_DECLARE(CubeMesh);

ECS_TYPE_DECLARE(CubeType);
ECS_TYPE_DECLARE(RectType);
ECS_TYPE_DECLARE(TriangleType);

void render_rectangle(ecs_iter_t* it) {
    mat4 rot_matrix, move_matrix, scale_matrix, proj_matrix, lookat_matrix, cam_rot_matrix;
    RectangleMesh* mesh = ecs_column(it, RectangleMesh, 1);
    Transform* transform = ecs_column(it, Transform, 2);
    int i;
    for (i = 0; i < it->count; i++, mesh++, transform++) {
        create_transform_matrix(transform,  &rot_matrix, &move_matrix,
                                            &scale_matrix, &proj_matrix,
                                            &lookat_matrix, &cam_rot_matrix);
        use_shader(mesh->shader_program, mesh->color,
                   rot_matrix, move_matrix,
                   scale_matrix, proj_matrix,
                   lookat_matrix, cam_rot_matrix,
                   mesh->albedoTex, mesh->heightTex,
                   mesh->metallicTex, mesh->normalTex,
                   mesh->roughnessTex, mesh->aoTex);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void render_triangle(ecs_iter_t* it) {
    mat4 rot_matrix, move_matrix, scale_matrix, proj_matrix, lookat_matrix, cam_rot_matrix;
    TriangleMesh* mesh = ecs_column(it, TriangleMesh, 1);
    Transform* transform = ecs_column(it, Transform, 2);
    int i;
    for (i = 0; i < it->count; i++, mesh++, transform++) {
        create_transform_matrix(transform,  &rot_matrix, &move_matrix,
                                &scale_matrix, &proj_matrix,
                                &lookat_matrix, &cam_rot_matrix);
        use_shader(mesh->shader_program, mesh->color,
                   rot_matrix, move_matrix,
                   scale_matrix, proj_matrix,
                   lookat_matrix, cam_rot_matrix,
                   mesh->albedoTex, mesh->heightTex,
                   mesh->metallicTex, mesh->normalTex,
                   mesh->roughnessTex, mesh->aoTex);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void render_cube(ecs_iter_t* it) {
    mat4 rot_matrix, move_matrix, scale_matrix, proj_matrix, lookat_matrix, cam_rot_matrix;
    CubeMesh* mesh = ecs_column(it, CubeMesh, 1);
    Transform* transform = ecs_column(it, Transform, 2);
    int i;
    for (i = 0; i < it->count; i++, mesh++, transform++) {
        create_transform_matrix(transform,  &rot_matrix, &move_matrix,
                                &scale_matrix, &proj_matrix,
                                &lookat_matrix, &cam_rot_matrix);
        use_shader(mesh->shader_program, mesh->color,
                   rot_matrix, move_matrix,
                   scale_matrix, proj_matrix,
                   lookat_matrix, cam_rot_matrix,
                   mesh->albedoTex, mesh->heightTex,
                   mesh->metallicTex, mesh->normalTex,
                   mesh->roughnessTex, mesh->aoTex);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void delete_triangle(ecs_iter_t* it) {
    TriangleMesh* mesh = ecs_column(it, TriangleMesh, 1);
    int i;
    for (i = 0; i < it->count; i++, mesh++) {
        glDeleteVertexArrays(1, &mesh->VAO);
        glDeleteBuffers(1, &mesh->VBO);
    }
}

void delete_rectangle(ecs_iter_t* it) {
    RectangleMesh* mesh = ecs_column(it, RectangleMesh, 1);
    int i;
    for (i = 0; i < it->count; i++, mesh++) {
        glDeleteVertexArrays(1, &mesh->VAO);
        glDeleteBuffers(1, &mesh->VBO);
    }
}

void delete_cube(ecs_iter_t* it) {
    CubeMesh* mesh = ecs_column(it, CubeMesh, 1);
    int i;
    for (i = 0; i < it->count; i++, mesh++) {
        glDeleteVertexArrays(1, &mesh->VAO);
        glDeleteBuffers(1, &mesh->VBO);
    }
}

int create_triangle(GLuint shader_program,
                    const Transform* transform,
                    GLuint albedoTex,
                    GLuint heightTex,
                    GLuint metallicTex,
                    GLuint normalTex,
                    GLuint roughnessTex,
                    GLuint aoTex) {
    GLuint VBO, VAO;
    float triangle_vertices[] = {
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
            0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f
    };
    load_vertices_to_buffers(triangle_vertices, sizeof(triangle_vertices), &VAO, &VBO);
    ecs_entity_t triangleEntity = ecs_new(world, TriangleType);
    ecs_set(world, triangleEntity, TriangleMesh, {
        .color = {1.0f, 1.0f, 1.0f, 0.5f},
        .shader_program = shader_program,
        .VBO = VBO,
        .VAO = VAO,
        .albedoTex = albedoTex,
        .heightTex = heightTex,
        .metallicTex = metallicTex,
        .normalTex = normalTex,
        .roughnessTex = roughnessTex,
        .aoTex = aoTex});
    ecs_set(world, triangleEntity, Transform, {
        .position = {transform->position[0], transform->position[1], transform->position[2]},
        .rotation = {transform->rotation[0], transform->rotation[1], transform->rotation[2]},
        .scale = {transform->scale[0], transform->scale[1], transform->scale[2]},
    });
    return 0;
}

int create_rectangle(GLuint shader_program,
                     const Transform* transform,
                     GLuint albedoTex,
                     GLuint heightTex,
                     GLuint metallicTex,
                     GLuint normalTex,
                     GLuint roughnessTex,
                     GLuint aoTex) {
    GLuint VBO, VAO;
    // Rectangle
    float rect_vertices[] = {
            0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

            0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f
    };
    load_vertices_to_buffers(rect_vertices, sizeof(rect_vertices), &VAO, &VBO);
    ecs_entity_t rectangleEntity = ecs_new(world, RectType);
    ecs_set(world, rectangleEntity, RectangleMesh, {.color = {1.0f, 1.0f, 1.0f, 1.0f},
                                                    .shader_program = shader_program,
                                                    .VBO = VBO,
                                                    .VAO = VAO,
                                                    .albedoTex = albedoTex,
                                                    .heightTex = heightTex,
                                                    .metallicTex = metallicTex,
                                                    .normalTex = normalTex,
                                                    .roughnessTex = roughnessTex,
                                                    .aoTex = aoTex});
    ecs_set(world, rectangleEntity, Transform, {
        .position = {transform->position[0], transform->position[1], transform->position[2]},
        .rotation = {transform->rotation[0], transform->rotation[1], transform->rotation[2]},
        .scale = {transform->scale[0], transform->scale[1], transform->scale[2]},
    });
    return 0;
}

int create_cube(GLuint shader_program,
                const Transform* transform,
                GLuint albedoTex,
                GLuint heightTex,
                GLuint metallicTex,
                GLuint normalTex,
                GLuint roughnessTex,
                GLuint aoTex) {
    GLuint VBO, VAO;
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,    0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,      0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,      1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,      1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,      1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,      0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,      0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   -1.0f, 0.0f, 0.0f,     1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,     1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,     0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,     1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, -1.0f, 0.0f,    1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,     0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f
    };
    load_vertices_to_buffers(vertices, sizeof(vertices), &VAO, &VBO);
    ecs_entity_t cubeEntity = ecs_new(world, CubeType);
    ecs_set(world, cubeEntity, CubeMesh, {
        .color = {1.0f, 1.0f, 1.0f, 1.f},
        .shader_program = shader_program,
        .VBO = VBO,
        .VAO = VAO,
        .albedoTex = albedoTex,
        .heightTex = heightTex,
        .metallicTex = metallicTex,
        .normalTex = normalTex,
        .roughnessTex = roughnessTex,
        .aoTex = aoTex});
    ecs_set(world, cubeEntity, Transform, {
        .position = {transform->position[0], transform->position[1], transform->position[2]},
        .rotation = {transform->rotation[0], transform->rotation[1], transform->rotation[2]},
        .scale = {transform->scale[0], transform->scale[1], transform->scale[2]},
    });
}

int init_simple_meshes(void) {
    ECS_COMPONENT_DEFINE(world, TriangleMesh);
    ECS_COMPONENT_DEFINE(world, RectangleMesh);
    ECS_COMPONENT_DEFINE(world, CubeMesh);
    ECS_TYPE_DEFINE(world, CubeType, CubeMesh, Transform);
    ECS_TYPE_DEFINE(world, RectType, RectangleMesh, Transform);
    ECS_TYPE_DEFINE(world, TriangleType, TriangleMesh, Transform);
    ECS_SYSTEM(world, render_rectangle, render_stage, RectangleMesh, Transform);
    ECS_TRIGGER(world, delete_rectangle, EcsOnRemove, RectangleMesh);
    ECS_SYSTEM(world, render_triangle, render_stage, TriangleMesh, Transform);
    ECS_TRIGGER(world, delete_triangle, EcsOnRemove, TriangleMesh);
    ECS_SYSTEM(world, render_cube, render_stage, CubeMesh, Transform);
    ECS_TRIGGER(world, delete_cube, EcsOnRemove, CubeMesh);
    return 0;
}

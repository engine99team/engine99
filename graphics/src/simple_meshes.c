#include "simple_meshes.h"
#include "globals.h"
#include "stages.h"

ECS_COMPONENT_DECLARE(TriangleMesh);
ECS_COMPONENT_DECLARE(RectangleMesh);

int create_transform_matrix(Transform* transform, struct mat4* result) {
    struct mat4 matrix, rot_matrix, proj_matrix;
    mat4_identity(matrix.v);
    mat4_rotation_quat(rot_matrix.v, transform->rotation.v);
    mat4_scale(matrix.v, matrix.v, transform->scale.v);
    mat4_multiply(matrix.v, rot_matrix.v, matrix.v);

    //mat4_translate(matrix.v, matrix.v, transform->position.v);
    mat4_perspective(proj_matrix.v, to_radians(60), 1.0, 0.1, 100.0);
    //mat4_multiply(matrix.v, proj_matrix.v, matrix.v);
    mat4_assign(result->v, matrix.v);
    return 0;
}

void render_rectangle(ecs_iter_t* it) {
    struct mat4 transform_matrix;
    RectangleMesh* mesh = ecs_column(it, RectangleMesh, 1);
    Transform* transform = ecs_column(it, Transform, 2);
    create_transform_matrix(transform, &transform_matrix);
    glUseProgram(mesh->shader_program);
    glUniform4f(glGetUniformLocation(mesh->shader_program, "color"), mesh->color.x, mesh->color.y, mesh->color.z, mesh->color.w);
    glUniformMatrix4fv(glGetUniformLocation(mesh->shader_program, "transform"), 1, GL_FALSE, transform_matrix.v);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void render_triangle(ecs_iter_t* it) {
    TriangleMesh* mesh = ecs_column(it, TriangleMesh, 1);
    glUseProgram(mesh->shader_program);
    glUniform4f(glGetUniformLocation(mesh->shader_program, "color"), mesh->color.x, mesh->color.y, mesh->color.z, mesh->color.w);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(mesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void delete_triangle(ecs_iter_t* it) {
    TriangleMesh* mesh = ecs_column(it, TriangleMesh, 1);
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
}

void delete_rectangle(ecs_iter_t* it) {
    RectangleMesh* mesh = ecs_column(it, RectangleMesh, 1);
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);
}

int create_triangle(GLuint shader_program, GLuint texture, const Transform* transform) {
    GLuint VBO, VAO;
    float triangle_vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f, 0.5f, 1.0f
    };
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    ECS_ENTITY(world, triangleEntity, TriangleMesh, Transform);
    ecs_set(world, triangleEntity, TriangleMesh, {.color = {0.0f, 0.0f, 0.0f, 1.0f},
        .shader_program = shader_program,
        .VBO = VBO,
        .VAO = VAO,
        .texture = texture});
    ecs_set(world, triangleEntity, Transform, {
        .position = {transform->position.v[0], transform->position.v[1], transform->position.v[2]},
        .rotation = {transform->rotation.v[0], transform->rotation.v[1], transform->rotation.v[2]},
        .scale = {transform->scale.v[0], transform->scale.v[1], transform->scale.v[2]},
    });
    return 0;
}

int create_rectangle(GLuint shader_program, GLuint texture, const Transform* transform) {
    GLuint VBO, VAO, EBO;
    // Rectangle
    float rect_vertices[] = {
            0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
            0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
    };

    unsigned int rect_indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    ECS_ENTITY(world, rectangleEntity, RectangleMesh, Transform);
    ecs_set(world, rectangleEntity, RectangleMesh, {.color = {0.0f, 0.0f, 0.0f, 1.0f},
                                                    .shader_program = shader_program,
                                                    .VBO = VBO,
                                                    .VAO = VAO,
                                                    .EBO = EBO,
                                                    .texture = texture});
    ecs_set(world, rectangleEntity, Transform, {
        .position = {transform->position.v[0], transform->position.v[1], transform->position.v[2]},
        .rotation = {transform->rotation.v[0], transform->rotation.v[1], transform->rotation.v[2]},
        .scale = {transform->scale.v[0], transform->scale.v[1], transform->scale.v[2]},
    });
    return 0;
}

int init_simple_meshes(void) {
    ECS_COMPONENT_DEFINE(world, TriangleMesh);
    ECS_COMPONENT_DEFINE(world, RectangleMesh);
    ECS_SYSTEM(world, render_rectangle, render_stage, RectangleMesh, Transform);
    ECS_TRIGGER(world, delete_rectangle, EcsOnRemove, RectangleMesh);
    ECS_SYSTEM(world, render_triangle, render_stage, TriangleMesh);
    ECS_TRIGGER(world, delete_triangle, EcsOnRemove, TriangleMesh);
    return 0;
}

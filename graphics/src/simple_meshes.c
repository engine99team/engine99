#include "simple_meshes.h"
#include "globals.h"
#include "stages.h"
#include <log.h>

ECS_COMPONENT_DECLARE(TriangleMesh);
ECS_COMPONENT_DECLARE(RectangleMesh);

void render_rectangle(ecs_iter_t* it) {
    RectangleMesh* mesh = ecs_column(it, RectangleMesh, 1);
    glUseProgram(mesh->shader_program);
    glUniform4f(glGetUniformLocation(mesh->shader_program, "color"), mesh->color.x, mesh->color.y, mesh->color.z, mesh->color.w);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->example_texture);
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
    glBindTexture(GL_TEXTURE_2D, mesh->example_texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(mesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void delete_triangle(ecs_iter_t* it) {
    TriangleMesh* mesh = ecs_column(it, TriangleMesh, 1);
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteTextures(1, &mesh->example_texture);
}

void delete_rectangle(ecs_iter_t* it) {
    RectangleMesh* mesh = ecs_column(it, RectangleMesh, 1);
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);
    glDeleteTextures(1, &mesh->example_texture);
}

int create_triangle(void) {
    GLuint shader_program, VBO, VAO, example_texture;
    float triangle_vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f, 0.5f, 1.0f
    };
    create_shader_program("shaders/example_shader.frag", "shaders/example_shader.vert", &shader_program);
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "ourTexture"), 0);
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
    load_png_texture("textures/cat.png", &example_texture);
    ECS_ENTITY(world, triangleEntity, TriangleMesh);
    ecs_set(world, triangleEntity, TriangleMesh, {.color = {0.0f, 0.0f, 0.0f, 1.0f},
        .shader_program = shader_program,
        .VBO = VBO,
        .VAO = VAO,
        .example_texture = example_texture});
    log_info("Triangle created");
    return 0;
}

int create_rectangle(void) {
    GLuint shader_program, VBO, VAO, EBO, example_texture;
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
    create_shader_program("shaders/example_shader.frag", "shaders/example_shader.vert", &shader_program);
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "ourTexture"), 0);
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
    load_png_texture("textures/cat.png", &example_texture);
    ECS_ENTITY(world, rectangleEntity, RectangleMesh);
    ecs_set(world, rectangleEntity, RectangleMesh, {.color = {0.0f, 0.0f, 0.0f, 1.0f},
                                                    .shader_program = shader_program,
                                                    .VBO = VBO,
                                                    .VAO = VAO,
                                                    .EBO = EBO,
                                                    .example_texture = example_texture});
    log_info("Rectangle created");
    return 0;
}

int simple_meshes_init(void) {
    ECS_COMPONENT_DEFINE(world, TriangleMesh);
    ECS_COMPONENT_DEFINE(world, RectangleMesh);
    ECS_SYSTEM(world, render_rectangle, render_stage, RectangleMesh);
    ECS_TRIGGER(world, delete_rectangle, EcsOnRemove, RectangleMesh);
    ECS_SYSTEM(world, render_triangle, render_stage, TriangleMesh);
    ECS_TRIGGER(world, delete_triangle, EcsOnRemove, TriangleMesh);

    return 0;
}
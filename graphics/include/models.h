#ifndef ENGINE99_MODELS_H
#define ENGINE99_MODELS_H
#include <flecs.h>
#include <log.h>
#include <graphics.h>

typedef struct Mesh {
    vec4 color;
    GLuint shader_program, VBO, VAO, texture;
} Mesh;

ECS_COMPONENT_EXTERN(Mesh);


int load_model(const char* filename, GLuint* VAO, GLuint* VBO);

#endif //ENGINE99_MODELS_H

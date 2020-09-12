#ifndef ENGINE99_SIMPLE_MESHES_H
#define ENGINE99_SIMPLE_MESHES_H
#include "core_components.h"
#include <flecs.h>
#include <graphics.h>
#include <cglm/cglm.h>
#include <cglm/quat.h>

typedef struct TriangleMesh {
    vec4 color;
    GLuint shader_program, VBO, VAO, texture;
} TriangleMesh;

typedef struct RectangleMesh {
    vec4 color;
    GLuint shader_program, VBO, VAO, EBO, texture;
} RectangleMesh;

int init_simple_meshes(void);
int create_rectangle(GLuint shader_program, GLuint texture, const Transform* transform);
int create_triangle(GLuint shader_program, GLuint texture, const Transform* transform);



#endif //ENGINE99_SIMPLE_MESHES_H

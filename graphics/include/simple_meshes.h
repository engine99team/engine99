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
    GLuint shader_program, VBO, VAO, texture;
} RectangleMesh;

typedef struct CubeMesh {
    vec4 color;
    GLuint shader_program, VBO, VAO, texture;
} CubeMesh;

ECS_COMPONENT_EXTERN(TriangleMesh);
ECS_COMPONENT_EXTERN(RectangleMesh);
ECS_COMPONENT_EXTERN(CubeMesh);

ECS_TYPE_EXTERN(CubeType);
ECS_TYPE_EXTERN(RectType);
ECS_TYPE_EXTERN(TriangleType);

int init_simple_meshes(void);
int create_rectangle(GLuint shader_program, GLuint texture, const Transform* transform);
int create_triangle(GLuint shader_program, GLuint texture, const Transform* transform);
int create_cube(GLuint shader_program, GLuint texture, const Transform* transform);

#endif //ENGINE99_SIMPLE_MESHES_H

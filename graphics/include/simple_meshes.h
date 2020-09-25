#ifndef ENGINE99_SIMPLE_MESHES_H
#define ENGINE99_SIMPLE_MESHES_H
#include "core_components.h"
#include <flecs.h>
#include <graphics.h>
#include <cglm/cglm.h>
#include <cglm/quat.h>

/**
 * Just simple triangle component
 */
typedef struct TriangleMesh {
    vec4 color;
    GLuint shader_program, VBO, VAO, texture;
} TriangleMesh;

/**
 * Just simple rectangle component
 */
typedef struct RectangleMesh {
    vec4 color;
    GLuint shader_program, VBO, VAO, texture;
} RectangleMesh;

/**
 * Just simple cube component
 */
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

/**
 * Inits simple meshes components and systems
 * @return
 */
int init_simple_meshes(void);

/**
 * Creates rectangle mesh entity
 * @param shader_program shader program, that is used to render mesh (created by create_shader_program)
 * @param texture texture, that is used to render mesh (created with load_png_texture)
 * @param transform transform component
 * @return zero if there is no problems
 */
int create_rectangle(GLuint shader_program, GLuint texture, const Transform* transform);

/**
 * Creates triangle mesh entity
 * @param shader_program shader program, that is used to render mesh (created by create_shader_program)
 * @param texture texture, that is used to render mesh (created with load_png_texture)
 * @param transform transform component
 * @return zero if there is no problems
 */
int create_triangle(GLuint shader_program, GLuint texture, const Transform* transform);

/**
 * Creates cube mesh entity
 * @param shader_program shader program, that is used to render mesh (created by create_shader_program)
 * @param texture texture, that is used to render mesh (created with load_png_texture)
 * @param transform transform component
 * @return zero if there is no problems
 */
int create_cube(GLuint shader_program, GLuint texture, const Transform* transform);

#endif //ENGINE99_SIMPLE_MESHES_H

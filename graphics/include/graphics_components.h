#ifndef ENGINE99_GRAPHICS_COMPONENTS_H
#define ENGINE99_GRAPHICS_COMPONENTS_H
#include "graphics.h"

/**
 * Texture component. Just has opengl texture id
 */
typedef struct Texture {
    GLuint id;
} Texture;

/**
 * Shader program component. Just has opengl shader program id
 */
typedef struct ShaderProgram {
    GLuint id;
} ShaderProgram;

/**
 * Camera component. Has fov, near and far parameters for render
 */
typedef struct Camera {
    float fov;
    float near;
    float far;
} Camera;

ECS_COMPONENT_EXTERN(Texture);
ECS_COMPONENT_EXTERN(ShaderProgram);
ECS_COMPONENT_EXTERN(Camera);

/**
 * Inits graphics components
 * @return zero if no problems
 */
int init_graphics_components(void);

#endif //ENGINE99_GRAPHICS_COMPONENTS_H

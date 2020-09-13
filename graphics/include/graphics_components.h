#ifndef ENGINE99_GRAPHICS_COMPONENTS_H
#define ENGINE99_GRAPHICS_COMPONENTS_H
#include "graphics.h"

typedef struct Texture {
    GLuint id;
} Texture;

typedef struct ShaderProgram {
    GLuint id;
} ShaderProgram;

typedef struct Camera {
    float fov;
    float near;
    float far;
} Camera;

ECS_COMPONENT_EXTERN(Texture);
ECS_COMPONENT_EXTERN(ShaderProgram);
ECS_COMPONENT_EXTERN(Camera);

int init_graphics_components(void);

#endif //ENGINE99_GRAPHICS_COMPONENTS_H

#ifndef ENGINE99_GRAPHICS_COMPONENTS_H
#define ENGINE99_GRAPHICS_COMPONENTS_H
#include "graphics.h"

typedef struct Texture {
    GLuint id;
} Texture;
typedef struct ShaderProgram {
    GLuint id;
} ShaderProgram;

ECS_COMPONENT_EXTERN(Texture);
ECS_COMPONENT_EXTERN(ShaderProgram);

int init_graphics_components(void);

#endif //ENGINE99_GRAPHICS_COMPONENTS_H

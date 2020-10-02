#ifndef ENGINE99_LIGHT_H
#define ENGINE99_LIGHT_H
#include <cglm/cglm.h>
#include <flecs.h>
#include "graphics.h"

typedef struct PointLight {
    vec3 radiance;
} PointLight;

typedef struct DirectLight {
    vec3 direction;
    vec3 radiance;
} DirectLight;

typedef struct SpotLight {
    vec3 direction;
    vec3 radiance;
    float angle;
} SpotLight;

ECS_COMPONENT_EXTERN(PointLight);
ECS_COMPONENT_EXTERN(DirectLight);
ECS_COMPONENT_EXTERN(SpotLight);

int init_lights (void);
int load_lights (GLuint shader_program);

#endif //ENGINE99_LIGHT_H

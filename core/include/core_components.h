#ifndef ENGINE99_CORE_COMPONENTS_H
#define ENGINE99_CORE_COMPONENTS_H
#include <flecs.h>
#include <cglm/cglm.h>

typedef struct Transform {
    // Position vector
    vec3 position;
    // Scale in local coordinates
    vec3 scale;
    // Rotation euler angles (in radians)
    vec3 rotation;
} Transform;

ECS_COMPONENT_EXTERN(Transform);

int init_core_components();

#endif //ENGINE99_CORE_COMPONENTS_H

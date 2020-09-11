#ifndef ENGINE99_CORE_COMPONENTS_H
#define ENGINE99_CORE_COMPONENTS_H
#include <flecs.h>
#include <mathc.h>

typedef struct Transform {
    // Position vector
    struct vec3 position;
    // Scale in local coordinates
    struct vec3 scale;
    // Rotation quaternion
    struct quat rotation;
} Transform;

ECS_COMPONENT_EXTERN(Transform);

int init_core_components();

#endif //ENGINE99_CORE_COMPONENTS_H

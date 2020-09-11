#ifndef ENGINE99_CORE_COMPONENTS_H
#define ENGINE99_CORE_COMPONENTS_H
#include <flecs.h>
#include <mathc.h>

typedef struct Transform {
    // Position vector
    mfloat_t position[VEC3_SIZE];
    // Scale in local coordinates
    mfloat_t scale[VEC3_SIZE];
    // Rotation in radians
    mfloat_t rotation[VEC3_SIZE];
} Transform;

ECS_COMPONENT_EXTERN(Transform);

int init_core_components();

#endif //ENGINE99_CORE_COMPONENTS_H

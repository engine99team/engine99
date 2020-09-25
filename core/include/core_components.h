#ifndef ENGINE99_CORE_COMPONENTS_H
#define ENGINE99_CORE_COMPONENTS_H
#include <flecs.h>
#include <cglm/cglm.h>

/**
 * Transform component
 */
typedef struct Transform {
    vec3 position; /*!< 3d position vector */
    vec3 scale; /*!< Scale in local coordinates */
    vec3 rotation; /*!< Rotation euler angles (in radians) */
} Transform;

ECS_COMPONENT_EXTERN(Transform);

/**
 * @brief Inits core components
 */
int init_core_components();

#endif //ENGINE99_CORE_COMPONENTS_H

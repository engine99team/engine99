#include "core_components.h"
#include "globals.h"

ECS_COMPONENT_DECLARE(Transform);

int init_core_components() {
    ECS_COMPONENT_DEFINE(world, Transform);
    return 0;
}
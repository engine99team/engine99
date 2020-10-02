#include "light.h"
#include "globals.h"

ECS_COMPONENT_DECLARE(PointLight);
ECS_COMPONENT_DECLARE(DirectLight);
ECS_COMPONENT_DECLARE(SpotLight);

int init_lights (void) {
    ECS_COMPONENT_DEFINE(world, PointLight);
    ECS_COMPONENT_DEFINE(world, DirectLight);
    ECS_COMPONENT_DEFINE(world, SpotLight);
    return 0;
}

int load_lights (GLuint shader_program) {
    int i, j;
    ecs_query_t* plQuery = ecs_query_new(world, "PointLight, Transform");
    ecs_query_t* dlQuery = ecs_query_new(world, "DirectLight");
    ecs_query_t* slQuery = ecs_query_new(world, "SpotLight, Transform");
    ecs_iter_t plIter = ecs_query_iter(plQuery);
    ecs_iter_t dlIter = ecs_query_iter(dlQuery);
    ecs_iter_t slIter = ecs_query_iter(slQuery);
    uint32_t plCount = 0;
    uint32_t dlCount = 0;
    uint32_t slCount = 0;
    i = 0;
    char buf[100];
    while (ecs_query_next(&plIter)) {
        plCount = plIter.count;
        glUniform1i(glGetUniformLocation(shader_program, "point_lights_count"), plCount);
        for (j = 0; j < plCount; j++) {
            PointLight* pointLight = ecs_column(&plIter + j, PointLight, 1);
            Transform* transform = ecs_column(&plIter + j, Transform, 2);
            sprintf(buf, "point_lights[%d].pos", i);
            glUniform3f(glGetUniformLocation(shader_program, buf), transform->position[0],
                        transform->position[1], transform->position[2]);
            sprintf(buf, "point_lights[%d].radiance", i);
            glUniform3f(glGetUniformLocation(shader_program, buf), pointLight->radiance[0],
                        pointLight->radiance[1], pointLight->radiance[2]);
            i++;
        }
    }
    i = 0;
    while (ecs_query_next(&dlIter)) {
        dlCount = dlIter.count;
        glUniform1i(glGetUniformLocation(shader_program, "direct_lights_count"), dlCount);
        for (j = 0; j < dlCount; j++) {
            DirectLight *directLight = ecs_column(&dlIter + j, DirectLight, 1);
            sprintf(buf, "direct_lights[%d].direction", i);
            glUniform3f(glGetUniformLocation(shader_program, buf), directLight->direction[0],
                        directLight->direction[1], directLight->direction[2]);
            sprintf(buf, "direct_lights[%d].radiance", i);
            glUniform3f(glGetUniformLocation(shader_program, buf), directLight->radiance[0],
                        directLight->radiance[1], directLight->radiance[2]);
            i++;
        }
    }
    i = 0;
    while (ecs_query_next(&slIter)) {
        slCount = slIter.count;
        glUniform1i(glGetUniformLocation(shader_program, "spot_lights_count"), slCount);
        for (j = 0; j < slCount; j++) {
            SpotLight* spotLight = ecs_column(&slIter + j, SpotLight, 1);
            Transform* transform = ecs_column(&slIter + j, Transform, 2);
            sprintf(buf, "spot_lights[%d].pos", i);
            glUniform3f(glGetUniformLocation(shader_program, buf), transform->position[0], transform->position[1],
                        transform->position[2]);
            sprintf(buf, "spot_lights[%d].radiance", i);
            glUniform3f(glGetUniformLocation(shader_program, buf), spotLight->radiance[0], spotLight->radiance[1],
                        spotLight->radiance[2]);
            sprintf(buf, "spot_lights[%d].direction", i);
            glUniform3f(glGetUniformLocation(shader_program, buf), spotLight->direction[0], spotLight->direction[1],
                        spotLight->direction[2]);
            sprintf(buf, "spot_lights[%d].angle", i);
            glUniform1f(glGetUniformLocation(shader_program, buf), spotLight->angle);
        }
    }
}
#ifndef ENGINE99_MODELS_H
#define ENGINE99_MODELS_H
#include <flecs.h>
#include <log.h>
#include <graphics.h>

typedef struct Mesh {
    vec4 color;
    GLuint shader_program, VBO, VAO, texture;
    uint32_t num_triangles;
} Mesh;

ECS_COMPONENT_EXTERN(Mesh);


int load_model(const char* filename, GLuint* VAO, GLuint* VBO, uint32_t* num_triangles);
int set_mesh_component(ecs_entity_t entity,
                       GLuint VAO,
                       GLuint VBO,
                       GLuint shader_program,
                       GLuint texture,
                       vec4 color,
                       uint32_t num_triangles);
int init_models (void);

#endif //ENGINE99_MODELS_H

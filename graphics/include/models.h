#ifndef ENGINE99_MODELS_H
#define ENGINE99_MODELS_H
#include <flecs.h>
#include <log.h>
#include <graphics.h>

/**
 * Mesh component. Renders triangulated model
 */
typedef struct Mesh {
    vec4 color;
    GLuint shader_program, VBO, VAO, texture;
    uint32_t num_triangles;
} Mesh;

ECS_COMPONENT_EXTERN(Mesh);

/**
 * Loads 3d obj model
 * @param[in] filename path to obj file
 * @param[out] VAO vertex array
 * @param[out] VBO vertex buffer
 * @param[out] num_triangles how much triangles has model
 * @return
 */
int load_model(const char* filename, GLuint* VAO, GLuint* VBO, uint32_t* num_triangles);

/**
 * Creates Mesh component for entity and sets it's parameters
 * @param[in, out] entity Entity to what we set Mesh component
 * @param[in] VAO vertex array object for Mesh component
 * @param[in] VBO vertex buffer for Mesh component
 * @param[in] shader_program shader program for Mesh
 * @param[in] texture texture for Mesh
 * @param[in] color color for Mesh
 * @param[in] num_triangles how much triangles has model
 * @return
 */
int set_mesh_component(ecs_entity_t entity,
                       GLuint VAO,
                       GLuint VBO,
                       GLuint shader_program,
                       GLuint texture,
                       vec4 color,
                       uint32_t num_triangles);

/**
 * Init Mesh component and systems
 * @return
 */
int init_models (void);

#endif //ENGINE99_MODELS_H

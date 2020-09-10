#ifndef ENGINE99_SIMPLE_MESHES_H
#define ENGINE99_SIMPLE_MESHES_H
#include <flecs.h>
#include <graphics.h>
#include <mathc.h>

typedef struct TriangleMesh {
    struct vec4 color;
    GLuint shader_program, VBO, VAO, example_texture;
} TriangleMesh;

typedef struct RectangleMesh {
    struct vec4 color;
    GLuint shader_program, VBO, VAO, EBO, example_texture;
} RectangleMesh;

int simple_meshes_init(void);
int create_rectangle(void);
int create_triangle(void);



#endif //ENGINE99_SIMPLE_MESHES_H

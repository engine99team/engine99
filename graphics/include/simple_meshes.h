#ifndef ENGINE99_SIMPLE_MESHES_H
#define ENGINE99_SIMPLE_MESHES_H
#include <flecs.h>
#include <graphics.h>
#include <mathc.h>

typedef struct TriangleMesh {
    struct vec4 color;
    GLuint shader_program, VBO, VAO, texture;
} TriangleMesh;

typedef struct RectangleMesh {
    struct vec4 color;
    GLuint shader_program, VBO, VAO, EBO, texture;
} RectangleMesh;

int init_simple_meshes(void);
int create_rectangle(GLuint shader_program, GLuint texture);
int create_triangle(GLuint shader_program, GLuint texture);



#endif //ENGINE99_SIMPLE_MESHES_H

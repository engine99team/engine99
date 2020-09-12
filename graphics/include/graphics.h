#ifndef ENGINE99_GRAPHICS_H
#define ENGINE99_GRAPHICS_H

#if __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include "globals.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <spng.h>
#include <cglm/cglm.h>
#include "core_components.h"
#include "config.h"

int load_shader(const char* filepath, GLenum shader_type, GLuint* shader);
int create_shader_program(const char* frag_filepath, const char* vert_filepath, GLuint* shader_program);
int load_png_data (const char* filepath, uint32_t* width, uint32_t* height, unsigned char **data);
int load_png_texture(const char* filepath, GLuint* texture);
int create_transform_matrix(Transform* transform, mat4* result);

#endif //ENGINE99_GRAPHICS_H

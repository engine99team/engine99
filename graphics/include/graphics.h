#ifndef ENGINE99_GRAPHICS_H
#define ENGINE99_GRAPHICS_H

#include "globals.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <spng.h>

int load_shader(const char* filepath, GLenum shader_type, GLuint* shader);
int create_shader_program(const char* frag_filepath, const char* vert_filepath, GLuint* shader_program);
int load_png_data (const char* filepath, uint32_t* width, uint32_t* height, unsigned char **data);
int load_png_texture(const char* filepath, GLuint* texture);

#endif //ENGINE99_GRAPHICS_H

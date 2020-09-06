#ifndef ENGINE99_GRAPHICS_H
#define ENGINE99_GRAPHICS_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

int load_shader(const char* filepath, GLenum shader_type, GLuint* shader);
int create_shader_program(const char* frag_filepath, const char* vert_filepath, GLuint* shader_program);


#endif //ENGINE99_GRAPHICS_H

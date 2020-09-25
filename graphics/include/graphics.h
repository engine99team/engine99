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

/**
 * Load shader from file to opengl
 * @param[in] filepath path to shader file
 * @param[in] shader_type vertex or fragment shader
 * @param[out] shader opengl id to shader
 * @return -1 if can't allocate buffer or can't load shader data
 */
int load_shader(const char* filepath, GLenum shader_type, GLuint* shader);

/**
 * Compiles shaders and links fragment shader and vertex shader to shader program
 * @param[in] frag_filepath file path to fragment shader
 * @param[in] vert_filepath file path to vertex shader
 * @param[out] shader_program opengl shader program id
 * @return -1 if can't load shaders or fail to link shaders to shader program
 */
int create_shader_program(const char* frag_filepath, const char* vert_filepath, GLuint* shader_program);

/**
 * Loads png image to data buffer
 * @param[in] filepath path to png file
 * @param[out] width output png width
 * @param[out] height output png height
 * @param[out] data output png data
 * @return -1 if can't init SPNG context or erors with png file(can't open or failed to load or get size)
 * or can't allocate a buffer or problems with spng_decode_image()
 */
int load_png_data (const char* filepath, uint32_t* width, uint32_t* height, unsigned char **data);

/**
 * Loading png from file to opengl texture
 * @param[in] filepath path to texture file
 * @param[out] texture opengl texture id
 * @return -1 if can't read png data
 */
int load_png_texture(const char* filepath, GLuint* texture);

/**
 * Creating trasformation matrix from transform for rendering
 * @param[in] transform object's transform
 * @param[out] result output matrix
 * @return
 */
int create_transform_matrix(Transform* transform, mat4* result);

/**
 * Loads vertices array to buffers
 * @param[in] vertices
 * @param[in] sizeof_vertices
 * @param[out] VAO
 * @param[out] VBO
 * @return zero if no problems
 */
int load_vertices_to_buffers (const float* vertices, size_t sizeof_vertices,GLuint* VAO, GLuint* VBO);

#endif //ENGINE99_GRAPHICS_H

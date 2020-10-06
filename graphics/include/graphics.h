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
 * Creates matrices from transform
 * @param[in] transform Transform component
 * @param[out] rot_matrix output rotation matrix
 * @param[out] move_matrix output movement matrix
 * @param[out] scale_matrix output scale matrix
 * @param[out] proj_matrix output projection matrix (based on Camera entity)
 * @param[out] lookat_matrix output lookat matrix (uses Camera entity too)
 * @param[out] cam_rot_matrix output Camera rotation matrix
 * @return 0
 */
int create_transform_matrix(Transform* transform,   mat4* rot_matrix,
                            mat4* move_matrix,
                            mat4* scale_matrix,
                            mat4* proj_matrix,
                            mat4* lookat_matrix,
                            mat4* cam_rot_matrix,
                            vec3* camera_position);

/**
 * Loads vertices array to buffers
 * @param[in] vertices
 * @param[in] sizeof_vertices
 * @param[out] VAO
 * @param[out] VBO
 * @return zero if no problems
 */
int load_vertices_to_buffers (const float* vertices, size_t sizeof_vertices,GLuint* VAO, GLuint* VBO);

int use_shader(GLuint shader_program,
               vec3 camera_position,
               vec4 color,
               mat4 rot_matrix,
               mat4 move_matrix,
               mat4 scale_matrix,
               mat4 proj_matrix,
               mat4 lookat_matrix,
               mat4 cam_rot_matrix,
               GLuint albedoTex,
               GLuint heightTex,
               GLuint metallicTex,
               GLuint normalTex,
               GLuint roughnessTex,
               GLuint aoTex);

int init_graphics (void);

#endif //ENGINE99_GRAPHICS_H

#include "graphics.h"
#include <log.h>
#include <stdlib.h>
#include "graphics_components.h"

/**
 * @brief returns size of file writes on pointer file_size
 * @param filepath path to texture file
 * @param file_size pointer to int
 * @return 0 if working correct, -1 if  can't open shader file
 */
int get_size_of_file(const char* filepath, int32_t* file_size) {
    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        log_error("Can't open shader file: %s", filepath);
        return -1;
    }
    fseek(file, 0L, SEEK_END);
    *file_size = ftell(file);
    fclose(file);
    return 0;
}

/**
 * @brief reads shader to buffer from file
 * @param filepath path to file with shader
 * @param buffer pointer to buffer
 * @param size size of buffer
 * @return 0 if working correct, -1 if  can't open shader file
 */
int read_from_file(const char* filepath, GLchar* buffer, GLint size) {
    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        log_error("Can't open shader file: %s", filepath);
        return -1;
    }
    fread(buffer, size, 1, file);
    buffer[size] = 0;
    fclose(file);
    return 0;
}

/**
 * @brief load shader from file to int* shader
 * @param filepath path to shader file
 * @param shader_type type of shader:
 * @param shader pointer on shader
 * @return -1 if can't allocate buffer or can't load shader data
 */
int load_shader(const char* filepath, GLenum shader_type, GLuint* shader) {
    GLuint local_shader;
    int32_t status;
    GLchar* buffer;
    GLint buffer_size;
    get_size_of_file(filepath, &buffer_size);
    buffer = (GLchar*) malloc((buffer_size+1) * sizeof(GLchar));
    if (buffer == NULL) {
        log_error("Can't allocate buffer");
        return -1;
    }
    read_from_file(filepath, buffer, buffer_size);
    local_shader = glCreateShader(shader_type);
    glShaderSource(local_shader, 1, (const GLchar**)&buffer, &buffer_size);
    glCompileShader(local_shader);
    glGetShaderiv(local_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetShaderInfoLog(local_shader, 512, NULL, info_log);
        free(buffer);
        log_error("Can't load shader from file: %s. %s", filepath, info_log);
        return -1;
    }
    free(buffer);
    *shader = local_shader;
    return 0;
}

/**
 * @brief linking fragment shader and vertex shader to shader_program
 * @param frag_filepath filepath to fragment shader
 * @param vert_filepath filepath to vertex shader
 * @param shader_program out  - pointer to shader_program
 * @return -1 if can't load shaders or fail to link shaders to shader program
 */
int create_shader_program(const char* frag_filepath, const char* vert_filepath, GLuint* shader_program) {
    GLuint frag_shader, vert_shader, local_shader_program;
    uint32_t res;
    int32_t status;
    res = load_shader(frag_filepath, GL_FRAGMENT_SHADER, &frag_shader);
    if (res != 0) {
        log_error("Can't load fragmental shader");
        return -1;
    }
    res = load_shader(vert_filepath, GL_VERTEX_SHADER, &vert_shader);
    if (res != 0) {
        log_error("Can't load vertex shader");
        return -1;
    }

    local_shader_program = glCreateProgram();
    glAttachShader(local_shader_program, vert_shader);
    glAttachShader(local_shader_program, frag_shader);
    glLinkProgram(local_shader_program);
    glGetProgramiv(local_shader_program, GL_LINK_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetProgramInfoLog(local_shader_program, 512, NULL, info_log);
        log_error("Failed to link shaders to shader program. %s", info_log);
        glDeleteProgram(local_shader_program);
        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
        return -1;
    }
    *shader_program = local_shader_program;
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    glUseProgram(local_shader_program);
    glUniform1i(glGetUniformLocation(local_shader_program, "ourTexture"), 0);
    ECS_ENTITY(world, shaderProgramEntity, ShaderProgram);
    ecs_set(world, shaderProgramEntity, ShaderProgram, {local_shader_program});
    return 0;
}

/**
 * @brief loads png to memory
 * @param filepath path to png file
 * @param width png width
 * @param height png height
 * @param data out
 * @return -1 if can't init SPNG context or erors with png file(can't open or failed to load or get size) or can't allocate a buffer or problems with spng_decode_image()
 */
int load_png_data (const char* filepath, uint32_t* width, uint32_t* height, unsigned char **data) {
    spng_ctx *png_ctx = spng_ctx_new(0);
    if (png_ctx == NULL) {
        log_fatal("Can't init SPNG context");
        return -1;
    }
    FILE* png_file;
    unsigned char *out_data;
    size_t out_size, out_width;
    int res;
    png_file = fopen(filepath, "rb");
    if  (png_file == NULL) {
        log_error("Can't open png file: %s", filepath);
        spng_ctx_free(png_ctx);
        return -1;
    }
    res = spng_set_png_file(png_ctx, png_file);
    if (res != 0) {
        log_error("Error while loading png file: %s", spng_strerror(res));
        spng_ctx_free(png_ctx);
        fclose(png_file);
        return -1;
    }
    struct spng_ihdr ihdr;
    res = spng_get_ihdr(png_ctx, &ihdr);
    if (res != 0) {
        log_error("Error while getting size of png file: %s", spng_strerror(res));
        spng_ctx_free(png_ctx);
        fclose(png_file);
        return -1;
    }
    res = spng_decoded_image_size(png_ctx, SPNG_FMT_RGBA8, &out_size);
    *height = ihdr.height;
    *width =  ihdr.width;
    if (res != 0) {
        log_error("Error while getting size of png file: %s", spng_strerror(res));
        spng_ctx_free(png_ctx);
        fclose(png_file);
        return -1;
    }
    out_data = (unsigned char *)malloc(*width * *height * 4);
    if (out_data == NULL) {
        log_error("Can't allocate memory for png buffer");
        spng_ctx_free(png_ctx);
        fclose(png_file);
        return -1;
    }
    res = spng_decode_image(png_ctx, NULL, 0, SPNG_FMT_RGBA8, SPNG_DECODE_PROGRESSIVE);
    if(res != 0)
    {
        log_error("progressive spng_decode_image() error: %s", spng_strerror(res));
        spng_ctx_free(png_ctx);
        free(out_data);
        fclose(png_file);
        return -1;
    }
    out_width = out_size / ihdr.height;
    struct spng_row_info row_info = {0};
    do {
        res = spng_get_row_info(png_ctx, &row_info);
        if(res) break;
        res = spng_decode_row(png_ctx, out_data + out_size - (row_info.row_num + 1) * out_width, out_width);
    }
    while(!res);

    if(res != SPNG_EOI)
    {
        log_error("progressive decode error: %s\n", spng_strerror(res));
        spng_ctx_free(png_ctx);
        free(out_data);
        fclose(png_file);
        return -1;
    }
    spng_ctx_free(png_ctx);
    *data = out_data;
    fclose(png_file);
    return 0;
}

/**
 * @brief loading png to texture
 * @param filepath in
 * @param texture out
 * @return -1 if can't read png data
 */
int load_png_texture(const char* filepath, GLuint* texture) {
    uint32_t width, height;
    int res;
    unsigned char* data;
    GLuint local_texture;
    res = load_png_data(filepath, &width, &height, &data);
    if (res != 0) {
        log_error("Can't load png data");
        return -1;
    }
    glGenTextures(1, &local_texture);
    glBindTexture(GL_TEXTURE_2D, local_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ECS_ENTITY(world, textureEntity, Texture);
    ecs_set(world, textureEntity, Texture, {local_texture});
    *texture = local_texture;
    free(data);
    return 0;
}

/**
 * @brief creating trasform matrix from transform
 * @param transform camera transformation
 * @param result out
 * @return
 */
int create_transform_matrix(Transform* transform, mat4* result) {
    mat4 matrix, rot_matrix, move_matrix, scale_matrix, proj_matrix, lookat_matrix, cam_rot_matrix;
    Camera* cam;
    Transform* camTrans;
    ecs_query_t *query = ecs_query_new(world, "Camera, Transform");
    ecs_iter_t it = ecs_query_iter(query);
    while (ecs_query_next(&it)) {
        cam = ecs_column(&it, Camera, 1);
        camTrans = ecs_column(&it, Transform, 2);
    }
    glm_scale_make(scale_matrix, transform->scale);
    glm_euler(transform->rotation, rot_matrix);
    glm_translate_make(move_matrix, transform->position);
    glm_perspective(cam->fov,
                    (float)CONFIG_WINDOW_WIDTH/CONFIG_WINDOW_HEIGHT,
                    cam->near,
                    cam->far,
                    proj_matrix);
    vec3 front = {0, 0 ,-1.f};
    vec3 up = {0, 1.f, 0};
    vec3 camDirection, camUp;
    glm_euler_yxz(camTrans->rotation, cam_rot_matrix);
    glm_mat4_mulv3(cam_rot_matrix, front, 0, camDirection);
    glm_mat4_mulv3(cam_rot_matrix, up,0, camUp);
    glm_look(camTrans->position, camDirection, camUp, lookat_matrix);
    glm_mat4_mulN((mat4 *[]){&proj_matrix, &lookat_matrix, &move_matrix, &rot_matrix, &scale_matrix}, 5, matrix);
    glm_mat4_copy(matrix, *result);
    return 0;
}

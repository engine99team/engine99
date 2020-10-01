#include "graphics.h"
#include <log.h>
#include <stdlib.h>
#include "graphics_components.h"

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
    glUniform1i(glGetUniformLocation(local_shader_program, "albedoTexture"), 0);
    glUniform1i(glGetUniformLocation(local_shader_program, "normalTexture"), 1);
    glUniform1i(glGetUniformLocation(local_shader_program, "heightTexture"), 2);
    glUniform1i(glGetUniformLocation(local_shader_program, "roughnessTexture"), 3);
    glUniform1i(glGetUniformLocation(local_shader_program, "metallicTexture"), 4);
    glUniform1i(glGetUniformLocation(local_shader_program, "aoTexture"), 5);
    ECS_ENTITY(world, shaderProgramEntity, ShaderProgram);
    ecs_set(world, shaderProgramEntity, ShaderProgram, {local_shader_program});
    return 0;
}

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

int load_vertices_to_buffers (const float* vertices, size_t sizeof_vertices,GLuint* VAO, GLuint* VBO) {
    GLuint lVAO, lVBO;
    glGenBuffers(1, &lVBO);
    glGenVertexArrays(1, &lVAO);

    glBindVertexArray(lVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(12 * sizeof(float)));
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    *VAO = lVAO;
    *VBO = lVBO;
    return 0;
}

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
               GLuint aoTex) {
    glUseProgram(shader_program);
    glUniform4f(glGetUniformLocation(shader_program, "color"), color[0], color[1], color[2], color[3]);
    glUniform3f(glGetUniformLocation(shader_program, "cameraPos"), camera_position[0], camera_position[1], camera_position[2]);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "rot_matrix"), 1, GL_FALSE, (GLfloat*)rot_matrix);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "move_matrix"), 1, GL_FALSE, (GLfloat*)move_matrix);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "scale_matrix"), 1, GL_FALSE, (GLfloat*)scale_matrix);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "proj_matrix"), 1, GL_FALSE, (GLfloat*)proj_matrix);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "lookat_matrix"), 1, GL_FALSE, (GLfloat*)lookat_matrix);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "cam_rot_matrix"), 1, GL_FALSE, (GLfloat*)cam_rot_matrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedoTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, heightTex);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughnessTex);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, metallicTex);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, aoTex);
    return 0;
}

int create_transform_matrix(Transform* transform,   mat4* rot_matrix,
                                                    mat4* move_matrix,
                                                    mat4* scale_matrix,
                                                    mat4* proj_matrix,
                                                    mat4* lookat_matrix,
                                                    mat4* cam_rot_matrix,
                                                    vec3* camera_position) {
    mat4 lrot_matrix, lmove_matrix, lscale_matrix, lproj_matrix, llookat_matrix, lcam_rot_matrix;
    Camera* cam;
    Transform* camTrans;
    ecs_query_t *query = ecs_query_new(world, "Camera, Transform");
    ecs_iter_t it = ecs_query_iter(query);
    while (ecs_query_next(&it)) {
        cam = ecs_column(&it, Camera, 1);
        camTrans = ecs_column(&it, Transform, 2);
    }
    glm_scale_make(lscale_matrix, transform->scale);
    glm_euler(transform->rotation, lrot_matrix);
    glm_translate_make(lmove_matrix, transform->position);
    glm_perspective(cam->fov,
                    (float)CONFIG_WINDOW_WIDTH/CONFIG_WINDOW_HEIGHT,
                    cam->near,
                    cam->far,
                    lproj_matrix);
    vec3 front = {0, 0 ,-1.f};
    vec3 up = {0, 1.f, 0};
    vec3 camDirection, camUp;
    glm_vec3_copy(camTrans->position, *camera_position);
    glm_euler_yxz(camTrans->rotation, lcam_rot_matrix);
    glm_mat4_mulv3(lcam_rot_matrix, front, 0, camDirection);
    glm_mat4_mulv3(lcam_rot_matrix, up,0, camUp);
    glm_look(camTrans->position, camDirection, camUp, llookat_matrix);
    glm_mat4_copy(lrot_matrix, *rot_matrix);
    glm_mat4_copy(lmove_matrix, *move_matrix);
    glm_mat4_copy(lscale_matrix, *scale_matrix);
    glm_mat4_copy(lproj_matrix, *proj_matrix);
    glm_mat4_copy(llookat_matrix, *lookat_matrix);
    glm_mat4_copy(lcam_rot_matrix, *cam_rot_matrix);
    return 0;
}

#include "graphics.h"
#include <log.h>

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
        return -1;
    }
    *shader_program = local_shader_program;
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    return 0;
}

int load_png_data (const char* filepath, uint32_t* width, uint32_t* height, unsigned char **data) {
    FILE* png_file;
    unsigned char *out_data;
    size_t out_size;
    int res;
    png_file = fopen(filepath, "rb");
    if  (png_file == NULL) {
        log_error("Can't open png file: %s", filepath);
        return -1;
    }
    res = spng_set_png_file(png_ctx, png_file);
    if (res != 0) {
        log_error("Error while loading png file: %s", spng_strerror(res));
        fclose(png_file);
        return -1;
    }
    struct spng_ihdr ihdr;
    res = spng_get_ihdr(png_ctx, &ihdr);
    if (res != 0) {
        log_error("Error while getting size of png file: %s", spng_strerror(res));
        fclose(png_file);
        return -1;
    }
    *height = ihdr.height;
    *width = ihdr.width;
    res = spng_decoded_image_size(png_ctx, SPNG_FMT_RGBA8, &out_size);
    if (res != 0) {
        log_error("Error while getting size of png file: %s", spng_strerror(res));
        fclose(png_file);
        return -1;
    }
    out_data = (unsigned char *)malloc(out_size);
    if (out_data == NULL) {
        fclose(png_file);
        log_error("Can't allocate memory for png buffer");
        return -1;
    }
    res = spng_decode_image(png_ctx, out_data, out_size, SPNG_FMT_RGBA8, 0);
    if (res != 0) {
        log_error("Error while decoding png file: %s", spng_strerror(res));
        free(out_data);
        fclose(png_file);
        return -1;
    }
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    *texture = local_texture;
    free(data);
    return 0;
}
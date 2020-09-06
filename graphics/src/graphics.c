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
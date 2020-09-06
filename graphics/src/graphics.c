#include "graphics.h"
#include <log.h>

int load_shader(const char* filepath, GLuint* shader) {
    int32_t result;
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        log_error("Can't open shader file: %s", filepath);
        return -1;
    }
    *shader = glCreateShader(GL_VERTEX_SHADER);
    fseek(file, 0L, SEEK_END);
    int32_t size = ftell(file);
    rewind(file);
    char* buffer = (char*) malloc((size+1) * sizeof(char));
    if (buffer == NULL) {
        log_error("Can't allocate buffer");
        return -1;
    }
    fgets(buffer, size, file);
    buffer[size] = '\0';
    glShaderSource(*shader, 1, (const char **)&buffer, &size);
    glCompileShader(*shader);
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        char info_log[512];
        glGetShaderInfoLog(*shader, 512, NULL, info_log);
        fclose(file);
        free(buffer);
        log_error("Can't load shader from file: %s. %s", filepath, info_log);
        return -1;
    }
    fclose(file);
    free(buffer);
    return 0;
}
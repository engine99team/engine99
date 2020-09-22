#include "models.h"
#include <tinyobj_loader_c.h>
#include <stdlib.h>

ECS_COMPONENT_DECLARE(Mesh);

void delete_mesh(ecs_iter_t* it) {
    Mesh* mesh = ecs_column(it, Mesh, 0);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->VAO);
}

void get_file_data(const char *filename, char **buf, size_t *len) {
    FILE* file;
    int i, res;
    char* buf1;
    size_t len1;
    file = fopen(filename, "rb");
    if (file == NULL) {
        log_error("Can't open file %s", filename);
        return;
    }
    res = fseek(file, 0, SEEK_END);
    if (res) {
        log_error("Can't get size of file %s", filename);
        return;
    }
    len1 = ftell(file);
    rewind(file);
    buf1 = (char*) malloc(((len1) + 1) * sizeof(char));
    fread(buf1, len1, 1, file);
    buf1[len1] = '\0';
    *buf = buf1;
    *len = len1;
}

int load_model(const char* filename, GLuint* VAO, GLuint* VBO) {
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;
    GLuint lVAO, lVBO, lEBO;
    int i, j;

    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                                &num_materials, filename, get_file_data, flags);
    if (ret != TINYOBJ_SUCCESS) {
        log_error("Error: %d", ret);
        return 1;
    }

    float vertices[attrib.num_face_num_verts * 5];

    for (i = 0; i < attrib.num_face_num_verts; i++) {
        //
    }

    glGenBuffers(1, &lVBO);
    glGenVertexArrays(1, &lVAO);

    glBindVertexArray(lVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    *VAO = lVAO;
    *VBO = lVBO;
    tinyobj_attrib_free(&attrib);
    tinyobj_shapes_free(shapes, num_shapes);
    tinyobj_materials_free(materials, num_materials);
    return 0;
}

int create_mesh() {

}

int init_models (void) {
    ECS_COMPONENT_DEFINE(world, Mesh);
    ECS_TRIGGER(world, delete_mesh, EcsOnRemove, Mesh);
}
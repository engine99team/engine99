#include "models.h"
#include <tinyobj_loader_c.h>
#include <stdlib.h>
#include "stages.h"

ECS_COMPONENT_DECLARE(Mesh);

void delete_mesh(ecs_iter_t* it) {
    Mesh* mesh = ecs_column(it, Mesh, 0);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->VAO);
}

void render_mesh(ecs_iter_t* it) {
    mat4 transform_matrix;
    Mesh* mesh = ecs_column(it, Mesh, 1);
    Transform* transform = ecs_column(it, Transform, 2);
    int i;
    for (i = 0; i < it->count; i++, mesh++, transform++) {
        create_transform_matrix(transform, &transform_matrix);
        glUseProgram(mesh->shader_program);
        glUniform4f(glGetUniformLocation(mesh->shader_program, "color"), mesh->color[0], mesh->color[1], mesh->color[2],
                    mesh->color[3]);
        glUniformMatrix4fv(glGetUniformLocation(mesh->shader_program, "transform"), 1, GL_FALSE,
                           (GLfloat *) transform_matrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->texture);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(mesh->VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh->num_triangles*3);
    }
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

int load_model(const char* filename, GLuint* VAO, GLuint* VBO, uint32_t* num_triangles) {
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

    float vertices[attrib.num_face_num_verts * 15];

    //for each face
    for (i = 0; i < attrib.num_face_num_verts; i++) {
        float* face = vertices + 15 * i;
        tinyobj_vertex_index_t* idx = attrib.faces + i*3;
        //for each vertex
        for (j = 0; j < 3; j++) {
            float* vertex = face + j*5;
            tinyobj_vertex_index_t idxj = idx[j];
            vertex[0] = attrib.vertices[idxj.v_idx*3];
            vertex[1] = attrib.vertices[idxj.v_idx*3 + 1];
            vertex[2] = attrib.vertices[idxj.v_idx*3 + 2];
            vertex[3] = attrib.texcoords[idxj.vt_idx*2];
            vertex[4] = attrib.texcoords[idxj.vt_idx*2 + 1];
        }
    }

    glGenBuffers(1, &lVBO);
    glGenVertexArrays(1, &lVAO);

    glBindVertexArray(lVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    *VAO = lVAO;
    *VBO = lVBO;
    *num_triangles = attrib.num_face_num_verts;
    tinyobj_attrib_free(&attrib);
    tinyobj_shapes_free(shapes, num_shapes);
    tinyobj_materials_free(materials, num_materials);
    return 0;
}

int set_mesh_component(ecs_entity_t entity,
                       GLuint VAO,
                       GLuint VBO,
                       GLuint shader_program,
                       GLuint texture,
                       vec4 color,
                       uint32_t num_triangles) {
    ecs_add(world, entity, Mesh);
    ecs_set(world, entity, Mesh, {
        .VAO = VAO,
        .VBO = VBO,
        .shader_program = shader_program,
        .texture = texture,
        .color = {color[0], color[1], color[2], color[3]},
        .num_triangles = num_triangles
    });
    return 0;
}

int init_models (void) {
    ECS_COMPONENT_DEFINE(world, Mesh);
    ECS_TRIGGER(world, delete_mesh, EcsOnRemove, Mesh);
    ECS_SYSTEM(world, render_mesh, render_stage, Mesh, Transform);
}
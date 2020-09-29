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
    mat4 rot_matrix, move_matrix, scale_matrix, proj_matrix, lookat_matrix, cam_rot_matrix;
    Mesh* mesh = ecs_column(it, Mesh, 1);
    Transform* transform = ecs_column(it, Transform, 2);
    int i;
    for (i = 0; i < it->count; i++, mesh++, transform++) {
        create_transform_matrix(transform,  &rot_matrix, &move_matrix,
                                &scale_matrix, &proj_matrix,
                                &lookat_matrix, &cam_rot_matrix);
        use_shader(mesh->shader_program, mesh->color,
                   rot_matrix, move_matrix,
                   scale_matrix, proj_matrix,
                   lookat_matrix, cam_rot_matrix);
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
        fclose(file);
        return;
    }
    len1 = ftell(file);
    rewind(file);
    buf1 = (char*) malloc(((len1) + 1) * sizeof(char));
    fread(buf1, len1, 1, file);
    buf1[len1] = '\0';
    *buf = buf1;
    *len = len1;
    fclose(file);
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

    float vertices[attrib.num_face_num_verts * 24];

    //for each face
    for (i = 0; i < attrib.num_face_num_verts; i++) {
        float* face = vertices + 24 * i;
        tinyobj_vertex_index_t* idx = attrib.faces + i*3;
        //for each vertex
        for (j = 0; j < 3; j++) {
            float* vertex = face + j*8;
            tinyobj_vertex_index_t idxj = idx[j];
            vertex[0] = attrib.vertices[idxj.v_idx*3];
            vertex[1] = attrib.vertices[idxj.v_idx*3 + 1];
            vertex[2] = attrib.vertices[idxj.v_idx*3 + 2];
            vertex[3] = attrib.normals[idxj.vn_idx*3];
            vertex[4] = attrib.normals[idxj.vn_idx*3 + 1];
            vertex[5] = attrib.normals[idxj.vn_idx*3 + 2];
            vertex[6] = attrib.texcoords[idxj.vt_idx*2];
            vertex[7] = attrib.texcoords[idxj.vt_idx*2 + 1];
        }
    }
    load_vertices_to_buffers(vertices, sizeof(vertices), &lVAO, &lVBO);
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
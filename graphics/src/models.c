#include "models.h"
#include <tinyobj_loader_c.h>
#include <stdlib.h>
#include "stages.h"

ECS_COMPONENT_DECLARE(Mesh);

static GLuint emptyTexture;

void delete_mesh(ecs_iter_t* it) {
    Mesh* mesh = ecs_column(it, Mesh, 0);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->VAO);
}

void render_mesh(ecs_iter_t* it) {
    mat4 rot_matrix, move_matrix, scale_matrix, proj_matrix, lookat_matrix, cam_rot_matrix;
    vec3 camera_position;
    Mesh* mesh = ecs_column(it, Mesh, 1);
    Transform* transform = ecs_column(it, Transform, 2);
    int i;
    for (i = 0; i < it->count; i++, mesh++, transform++) {
        create_transform_matrix(transform,  &rot_matrix, &move_matrix,
                                &scale_matrix, &proj_matrix,
                                &lookat_matrix, &cam_rot_matrix, &camera_position);
        use_shader(mesh->shader_program, camera_position, mesh->color,
                   rot_matrix, move_matrix,
                   scale_matrix, proj_matrix,
                   lookat_matrix, cam_rot_matrix,
                   mesh->albedoTex, mesh->heightTex,
                   mesh->metallicTex, mesh->normalTex,
                   mesh->roughnessTex, mesh->aoTex);
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

    float vertices[attrib.num_face_num_verts * 42];

    //for each face
    for (i = 0; i < attrib.num_face_num_verts; i++) {
        vec3 tangent;
        vec3 bitangent;
        vec3 edge1;
        vec3 edge2;
        vec2 deltaUV1;
        vec2 deltaUV2;
        float* face = vertices + 42 * i;
        tinyobj_vertex_index_t* idx = attrib.faces + i*3;
        vec3 pos1 = {attrib.vertices[idx[0].v_idx*3],
                     attrib.vertices[idx[0].v_idx*3+1],
                     attrib.vertices[idx[0].v_idx*3+2]};
        vec3 pos2 = {attrib.vertices[idx[1].v_idx*3],
                     attrib.vertices[idx[1].v_idx*3+1],
                     attrib.vertices[idx[1].v_idx*3+2]};
        vec3 pos3 = {attrib.vertices[idx[2].v_idx*3],
                     attrib.vertices[idx[2].v_idx*3+1],
                     attrib.vertices[idx[2].v_idx*3+2]};

        vec2 uv1 = {attrib.texcoords[idx[0].vt_idx*2],
                    attrib.texcoords[idx[0].vt_idx*2+1]};
        vec2 uv2 = {attrib.texcoords[idx[1].vt_idx*2],
                    attrib.texcoords[idx[1].vt_idx*2+1]};
        vec2 uv3 = {attrib.texcoords[idx[2].vt_idx*2],
                    attrib.texcoords[idx[2].vt_idx*2+1]};

        glm_vec3_sub(pos2, pos1, edge1);
        glm_vec3_sub(pos3, pos1, edge2);
        glm_vec3_sub(uv2, uv1, deltaUV1);
        glm_vec3_sub(uv3, uv1, deltaUV2);

        float f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);
        tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
        tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
        tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
        glm_normalize(tangent);

        bitangent[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
        bitangent[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
        bitangent[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);
        glm_normalize(bitangent);

        //for each vertex
        for (j = 0; j < 3; j++) {
            float* vertex = face + j*14;
            tinyobj_vertex_index_t idxj = idx[j];
            vertex[0] = attrib.vertices[idxj.v_idx*3];
            vertex[1] = attrib.vertices[idxj.v_idx*3 + 1];
            vertex[2] = attrib.vertices[idxj.v_idx*3 + 2];

            vertex[3] = attrib.normals[idxj.vn_idx*3];
            vertex[4] = attrib.normals[idxj.vn_idx*3 + 1];
            vertex[5] = attrib.normals[idxj.vn_idx*3 + 2];

            vertex[6] = tangent[0];
            vertex[7] = tangent[1];
            vertex[8] = tangent[2];

            vertex[9] =  bitangent[0];
            vertex[10] = bitangent[1];
            vertex[11] = bitangent[2];

            vertex[12] = attrib.texcoords[idxj.vt_idx*2];
            vertex[13] = attrib.texcoords[idxj.vt_idx*2 + 1];
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
                       vec4 color,
                       uint32_t num_triangles,
                       GLuint albedoTex,
                       GLuint heightTex,
                       GLuint metallicTex,
                       GLuint normalTex,
                       GLuint roughnessTex,
                       GLuint aoTex) {
    ecs_add(world, entity, Mesh);
    if (albedoTex == 0) {
        albedoTex = emptyTexture;
    }
    if (heightTex == 0) {
        heightTex = emptyTexture;
    }
    if (metallicTex == 0) {
        metallicTex = emptyTexture;
    }
    if (normalTex == 0) {
        normalTex = emptyTexture;
    }
    if (roughnessTex == 0) {
        roughnessTex = emptyTexture;
    }
    if (aoTex == 0) {
        aoTex = emptyTexture;
    }

    ecs_set(world, entity, Mesh, {
        .VAO = VAO,
        .VBO = VBO,
        .shader_program = shader_program,
        .albedoTex = albedoTex,
        .heightTex = heightTex,
        .metallicTex = metallicTex,
        .normalTex = normalTex,
        .roughnessTex = roughnessTex,
        .aoTex = aoTex,
        .color = {color[0], color[1], color[2], color[3]},
        .num_triangles = num_triangles
    });
    return 0;
}

int init_models (void) {
    ECS_COMPONENT_DEFINE(world, Mesh);
    ECS_TRIGGER(world, delete_mesh, EcsOnRemove, Mesh);
    ECS_SYSTEM(world, render_mesh, render_stage, Mesh, Transform);
    glGenTextures(1, &emptyTexture);
    GLubyte data[] = { 255, 255, 255, 255 };
    glBindTexture(GL_TEXTURE_2D, emptyTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}
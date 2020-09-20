#include "example.h"
#include <nuklear_include.h>
#include "globals.h"
#include "stages.h"
#include "graphics.h"
#include "simple_meshes.h"
#include "core_components.h"
#include "graphics_components.h"
#include <log.h>
#include <models.h>

float axis_ws = 0, axis_ad = 0;
float   a = 0,
        w = 0,
        s = 0,
        d = 0;
vec3 rot;
bool isRotating;

int example_events (SDL_Event* event, float delta_time) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
                a = 1;
                break;
            case SDLK_d:
                d = 1;
                break;
            case SDLK_w:
                w = 1;
                break;
            case SDLK_s:
                s = 1;
                break;
        }
    } else if (event->type == SDL_MOUSEMOTION) {
        rot[0] = (float)event->motion.yrel;
        rot[1] = (float)event->motion.xrel;
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_RIGHT) {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            isRotating = true;
        }
    } else if (event->type == SDL_MOUSEBUTTONUP) {
        if (event->button.button == SDL_BUTTON_RIGHT) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            isRotating = false;
        }
    } else if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.sym) {
            case SDLK_a:
                a = 0;
                break;
            case SDLK_d:
                d = 0;
                break;
            case SDLK_w:
                w = 0;
                break;
            case SDLK_s:
                s = 0;
                break;
        }
    }
    axis_ws = (float)w - (float)s;
    axis_ad = (float)d - (float)a;
    return 0;
}

void camera_movement(ecs_iter_t* it) {
    Transform* camTrans;
    mat4 cam_rot;
    vec3 cam_front, cam_right;
    vec3 negZ = {0, 0, -1.f};
    vec3 x = {1.f, 0, 0};
    camTrans = ecs_column(it, Transform, 2);

    glm_euler_yxz(camTrans->rotation, cam_rot);
    glm_mat4_mulv3(cam_rot, negZ, 0, cam_front);
    glm_mat4_mulv3(cam_rot, x, 0, cam_right);
    vec3 delta = {0, 0, 0};
    float delta_time = 2 * it->delta_time;

    glm_vec3_scale(cam_right, axis_ad, cam_right);
    glm_vec3_scale(cam_front, axis_ws, cam_front);
    glm_vec3_add(delta, cam_right, delta);
    glm_vec3_add(delta, cam_front, delta);
    glm_vec3_normalize(delta);
    glm_vec3_scale(delta, delta_time, delta);
    glm_vec3_add(camTrans->position, delta, camTrans->position);
    //Rotation
    if (isRotating) {
        glm_vec3_scale(rot, -2 * CGLM_PI/180, rot);
        glm_vec3_add(camTrans->rotation, rot, camTrans->rotation);
        if (camTrans->rotation[0] > CGLM_PI_2)
            camTrans->rotation[0] = CGLM_PI_2;
        if (camTrans->rotation[0] < -CGLM_PI_2)
            camTrans->rotation[0] = -CGLM_PI_2;
        if (camTrans->rotation[1] > CGLM_PI)
            camTrans->rotation[1] = -CGLM_PI;
        if (camTrans->rotation[1] < -CGLM_PI)
            camTrans->rotation[1] = CGLM_PI;
    }
}

void example_imgui (ecs_iter_t* it) {
    if (nk_begin(nk_ctx, "Demo", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                 NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {
        enum {EASY, HARD};
        static int op = EASY;
        static int property = 20;

        nk_layout_row_static(nk_ctx, 30, 80, 1);
        if (nk_button_label(nk_ctx, "button"))
            printf("button pressed!\n");
        nk_layout_row_dynamic(nk_ctx, 30, 2);
        if (nk_option_label(nk_ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(nk_ctx, "hard", op == HARD)) op = HARD;
        nk_layout_row_dynamic(nk_ctx, 22, 1);
        nk_property_int(nk_ctx, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(nk_ctx, 20, 1);
        nk_label(nk_ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(nk_ctx, 25, 1);
        if (nk_combo_begin_color(nk_ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(nk_ctx),400))) {
            nk_layout_row_dynamic(nk_ctx, 120, 1);
            bg = nk_color_picker(nk_ctx, bg, NK_RGBA);
            nk_layout_row_dynamic(nk_ctx, 25, 1);
            bg.r = nk_propertyf(nk_ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
            bg.g = nk_propertyf(nk_ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
            bg.b = nk_propertyf(nk_ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
            bg.a = nk_propertyf(nk_ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(nk_ctx);
        }
    }
    nk_end(nk_ctx);
}

void example_imgui_rect (ecs_iter_t* it) {
    RectangleMesh* mesh = ecs_column(it, RectangleMesh, 1);
    int i;
    for (i = 0; i < it->count; i++, mesh++) {
        struct nk_colorf* rectangle_color = (struct nk_colorf*)&mesh->color;
        char window_title[30];
        sprintf(window_title, "Demo Rect %d", i);
        if (nk_begin(nk_ctx, window_title, nk_rect(50, 300, 230, 250),
                     NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                     NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(nk_ctx, 20, 1);
            nk_label(nk_ctx, "rectangle:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(nk_ctx, 25, 1);
            if (nk_combo_begin_color(nk_ctx, nk_rgb_cf(*rectangle_color), nk_vec2(nk_widget_width(nk_ctx), 400))) {
                nk_layout_row_dynamic(nk_ctx, 120, 1);
                *rectangle_color = nk_color_picker(nk_ctx, *rectangle_color, NK_RGBA);
                nk_layout_row_dynamic(nk_ctx, 25, 1);
                rectangle_color->r = nk_propertyf(nk_ctx, "#R:", 0, rectangle_color->r, 1.0f, 0.01f,0.005f);
                rectangle_color->g = nk_propertyf(nk_ctx, "#G:", 0, rectangle_color->g, 1.0f, 0.01f,0.005f);
                rectangle_color->b = nk_propertyf(nk_ctx, "#B:", 0, rectangle_color->b, 1.0f, 0.01f,0.005f);
                rectangle_color->a = nk_propertyf(nk_ctx, "#A:", 0, rectangle_color->a, 1.0f, 0.01f,0.005f);
                nk_combo_end(nk_ctx);
            }
        }
        nk_end(nk_ctx);
    }
}

void example_imgui_cube (ecs_iter_t* it) {
    CubeMesh* mesh = ecs_column(it, CubeMesh, 1);
    int i;
    for (i = 0; i < it->count; i++, mesh++) {
        struct nk_colorf *rectangle_color = (struct nk_colorf *) &mesh->color;
        char window_title[30];
        sprintf(window_title, "Demo Cube %d", i);
        if (nk_begin(nk_ctx, window_title, nk_rect(50, 300, 230, 250),
                     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
            nk_layout_row_dynamic(nk_ctx, 20, 1);
            nk_label(nk_ctx, "cube:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(nk_ctx, 25, 1);
            if (nk_combo_begin_color(nk_ctx, nk_rgb_cf(*rectangle_color), nk_vec2(nk_widget_width(nk_ctx), 400))) {
                nk_layout_row_dynamic(nk_ctx, 120, 1);
                *rectangle_color = nk_color_picker(nk_ctx, *rectangle_color, NK_RGBA);
                nk_layout_row_dynamic(nk_ctx, 25, 1);
                rectangle_color->r = nk_propertyf(nk_ctx, "#R:", 0, rectangle_color->r, 1.0f, 0.01f, 0.005f);
                rectangle_color->g = nk_propertyf(nk_ctx, "#G:", 0, rectangle_color->g, 1.0f, 0.01f, 0.005f);
                rectangle_color->b = nk_propertyf(nk_ctx, "#B:", 0, rectangle_color->b, 1.0f, 0.01f, 0.005f);
                rectangle_color->a = nk_propertyf(nk_ctx, "#A:", 0, rectangle_color->a, 1.0f, 0.01f, 0.005f);
                nk_combo_end(nk_ctx);
            }
        }
        nk_end(nk_ctx);
    }
}

void example_imgui_tri (ecs_iter_t* it) {
    TriangleMesh* mesh = ecs_column(it, TriangleMesh, 1);
    int i;
    for (i = 0; i < it->count; i++, mesh++) {
        struct nk_colorf *triangle_color = (struct nk_colorf *) &mesh->color;
        char window_title[30];
        sprintf(window_title, "Demo triangle %d", i);
        if (nk_begin(nk_ctx, window_title, nk_rect(280, 50, 230, 250),
                     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
            nk_layout_row_dynamic(nk_ctx, 20, 1);
            nk_label(nk_ctx, "triangle:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(nk_ctx, 25, 1);
            if (nk_combo_begin_color(nk_ctx, nk_rgb_cf(*triangle_color), nk_vec2(nk_widget_width(nk_ctx), 400))) {
                nk_layout_row_dynamic(nk_ctx, 120, 1);
                *triangle_color = nk_color_picker(nk_ctx, *triangle_color, NK_RGBA);
                nk_layout_row_dynamic(nk_ctx, 25, 1);
                triangle_color->r = nk_propertyf(nk_ctx, "#R:", 0, triangle_color->r, 1.0f, 0.01f, 0.005f);
                triangle_color->g = nk_propertyf(nk_ctx, "#G:", 0, triangle_color->g, 1.0f, 0.01f, 0.005f);
                triangle_color->b = nk_propertyf(nk_ctx, "#B:", 0, triangle_color->b, 1.0f, 0.01f, 0.005f);
                triangle_color->a = nk_propertyf(nk_ctx, "#A:", 0, triangle_color->a, 1.0f, 0.01f, 0.005f);
                nk_combo_end(nk_ctx);
            }
        }
        nk_end(nk_ctx);
    }
}

void example_imgui_fps(ecs_iter_t* it) {
    if (nk_begin(nk_ctx, "FPS counter", nk_rect(280, 300, 230, 100),
                 NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE)) {
        char buffer[10];
        float dt = it->delta_system_time;
        float fps = 1.0f/dt;
        sprintf(buffer, "%.2f", fps);
        nk_layout_row_dynamic(nk_ctx, 20, 1);
        nk_label(nk_ctx, buffer, NK_TEXT_ALIGN_LEFT);
    }
    nk_end(nk_ctx);
}

int init_example(void) {
    GLuint example_texture, shader_program;
    GLuint model_VAO,  model_EBO, model_VBO;
    ECS_SYSTEM(world, example_imgui, imgui_stage, global_tag)
    ECS_SYSTEM(world, example_imgui_fps, imgui_stage, global_tag)
    ECS_SYSTEM(world, example_imgui_rect, imgui_stage, RectangleMesh)
    ECS_SYSTEM(world, example_imgui_tri, imgui_stage, TriangleMesh)
    ECS_SYSTEM(world, example_imgui_cube, imgui_stage, CubeMesh)
    ECS_SYSTEM(world, camera_movement, update_stage, Camera, Transform)
    create_shader_program("shaders/example_shader.frag", "shaders/example_shader.vert", &shader_program);
    load_png_texture("textures/test.png", &example_texture);
    Transform transform1 = {
                .position = {0, 0.1f, -2.f},
                .rotation = {-CGLM_PI / 3, -CGLM_PI / 3, 0},
                .scale = {1, 1, 1}
            };
    create_cube(shader_program, example_texture, &transform1);
    Transform transform2 = {
            .position = {0, 0.1f, 0.f},
            .rotation = {CGLM_PI / 3, -CGLM_PI / 3, 0},
            .scale = {1, 1, 1}
    };
    create_cube(shader_program, example_texture, &transform2);

    load_model("models/test.obj", &model_VAO, &model_VBO, &model_EBO);
    return 0;
}

int destroy_example(void) {
    return 0;
}

#include "example.h"
#include <nuklear_include.h>
#include "globals.h"
#include "stages.h"
#include "graphics.h"
#include <log.h>

GLuint shader_program, VBO, VAO, EBO, example_texture;
float vertices[] = {
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
};

unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
};

struct nk_colorf rectangle_color = {0.5f, 0.1f, 0.2f, 1};

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
        nk_layout_row_dynamic(nk_ctx, 20, 1);
        nk_label(nk_ctx, "rectangle:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(nk_ctx, 25, 1);
        if (nk_combo_begin_color(nk_ctx, nk_rgb_cf(rectangle_color), nk_vec2(nk_widget_width(nk_ctx), 400))) {
            nk_layout_row_dynamic(nk_ctx, 120, 1);
            rectangle_color = nk_color_picker(nk_ctx, rectangle_color, NK_RGBA);
            nk_layout_row_dynamic(nk_ctx, 25, 1);
            rectangle_color.r = nk_propertyf(nk_ctx, "#R:", 0, rectangle_color.r, 1.0f, 0.01f,0.005f);
            rectangle_color.g = nk_propertyf(nk_ctx, "#G:", 0, rectangle_color.g, 1.0f, 0.01f,0.005f);
            rectangle_color.b = nk_propertyf(nk_ctx, "#B:", 0, rectangle_color.b, 1.0f, 0.01f,0.005f);
            rectangle_color.a = nk_propertyf(nk_ctx, "#A:", 0, rectangle_color.a, 1.0f, 0.01f,0.005f);
            nk_combo_end(nk_ctx);
        }
    }
    nk_end(nk_ctx);
}

void example_render_triangle(ecs_iter_t* it) {
    glUseProgram(shader_program);
    glUniform4f(glGetUniformLocation(shader_program, "color"), rectangle_color.r, rectangle_color.g, rectangle_color.b, rectangle_color.a);
    glUniform1i(glGetUniformLocation(shader_program, "ourTexture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, example_texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int init_example(void) {
    create_shader_program("shaders/example_shader.frag", "shaders/example_shader.vert", &shader_program);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    load_png_texture("textures/cat.png", &example_texture);
    ECS_SYSTEM(world, example_imgui, imgui_stage, global_tag)
    ECS_SYSTEM(world, example_render_triangle, render_stage, global_tag)
    return 0;
}

int destroy_example(void) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return 0;
}

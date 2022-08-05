#include "text.hpp"
#include <gl/texture.hpp>
#include <gl/shader.hpp>
#include <glad/glad.hpp>

const char* text_vs {
R"(#version 300 es
precision mediump float;
in vec2 a_vert;
in vec2 a_tex_coord;
out vec2 tex_coord;
uniform vec2 u_pos;
uniform vec2 u_offset;
uniform vec2 u_res;

void main() {
    tex_coord = a_tex_coord;
    gl_Position = vec4(((a_vert + u_offset + vec2(0.5, -0.5))/u_res.xy*30.f) + u_pos, 0.f, 1.f);
}
)"};

const char* text_fs {
R"(#version 300 es
precision mediump float;
in vec2 tex_coord;
out vec4 frag_color;

uniform sampler2D u_tex;
uniform float u_time;
uniform vec2 u_tex_size;
uniform vec2 u_tex_offset;

void main() {
    frag_color = texture(u_tex, tex_coord * u_tex_size + u_tex_offset);
    if (frag_color.a < 0.5f) {
        discard;
    }
}
)"};

std::initializer_list<float> text_vertices {
    -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,   1.0f, 1.0f,
    0.5f, 0.5f,    1.0f, 0.0f,
    -0.5f, 0.5f,   0.0f, 0.0f
};


Text::Text(std::string_view text, glm::vec2 pos) : text(text), pos(pos) {
}

void Text::create() {
    //shared_vbo = text_shader.setAttribute({""}, std::initializer_list<T> buffer)
}

void Text::update() {
    render();
}

void Text::render() {
   // text_shader.use();

    float x {};
    float y {};
    for (uint8_t c : text) {
        if (c == '\n') {
            x = 0;
            y-=1.5;
            continue;
        }
        glm::vec2 texture_size = (glm::vec2)current_texture->size;
        current_shader->uniform("u_tex_size", (glm::vec2)char_size/texture_size);

        uint32_t rows = current_texture->size.x/char_size.x;
        uint32_t px = c%rows*char_size.x;
        uint32_t py = c/rows*char_size.y;
        current_shader->uniform("u_tex_offset", glm::vec2(glm::vec2(px, py)/texture_size));
        current_shader->uniform("u_pos", pos);
        current_shader->uniform("u_offset", glm::vec2(x, y));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        x++;
    }
}

#pragma once
#include <gl/texture.hpp>
#include <gl/vao.hpp>
#include <gl/vbo.hpp>
#include <gl/shader.hpp>
#include <util/smartvector.hpp>

inline static const char* text_vs {
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
    gl_Position = vec4(((a_vert + u_offset + vec2(0.5, -0.5))/u_res.xy*100.f) + u_pos, 0.f, 1.f);

}
)"};

inline static const char* text_fs {
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

inline static std::initializer_list<float> text_vertices {
    -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,   1.0f, 1.0f,
    0.5f, 0.5f,    1.0f, 0.0f,
    -0.5f, 0.5f,   0.0f, 0.0f
};

struct TextManager;
struct Text {
    std::string text;
    glm::vec2 pos;
    glm::uvec2 char_size {8.f, 8.f};
    Text(std::string_view text, glm::vec2 pos = {-1, 1});

    using Manager = TextManager;

    void create(void);
    void update(void);
    void render(void);
};

struct TextManager {
    SmartVector<std::shared_ptr<Text>, true> texts;
    static constexpr FixedString texture_name { "font" };
    Shader shader { text_vs, text_fs };
    Texture* texture;
    VAO vao;
    VBO vbo {
        shader.setAttribute({"a_vert", "a_tex_coord"}, text_vertices)
    };

    TextManager(auto& s) {
        pretty(s);
        texture = &s.template getTexture<texture_name>();
    }

    void update() {
        shader.use();
        vao.use();
        texture->use();
        shader.uniform("u_tex_size", glm::vec2(1, 1));
        // Update entities
        for (auto& e : texts) {
            e->update();
        }
    }

    auto create(std::string_view text) {
        auto e = std::make_shared<Text>(text);
        texts.emplace_back(e);
        return e;
    }
};

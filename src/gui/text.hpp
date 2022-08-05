#pragma once
#include <gl/texture.hpp>
#include <gl/vao.hpp>
#include <gl/vbo.hpp>
#include <gl/shader.hpp>
#include <util/smartvector.hpp>

extern const char* text_vs;
extern const char* text_fs;
extern std::initializer_list<float> text_vertices;

struct TextManager;
struct Text {
    std::string text;
    glm::vec2 pos;
    glm::uvec2 char_size {8.f, 8.f};
    Text(std::string_view text, glm::vec2 pos = {-1, 1});

    using Manager = TextManager;

    void create();
    void update();
    void render();
};

struct TextManager {
    SmartVector<std::shared_ptr<Text>, true> texts;
    using texture_name = TextureEntry<"font">;
    Shader shader { text_vs, text_fs };
    Texture* texture;
    VAO vao;
    VBO vbo {
        shader.setAttribute({"a_vert", "a_tex_coord"}, text_vertices)
    };

    TextManager() {

    }

    void update() {
        shader.use();
        vao.use();
        texture->use();
        shader.uniform("u_tex_size", glm::vec2(1, 1));
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

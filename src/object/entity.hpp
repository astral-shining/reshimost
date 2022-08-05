#pragma once
#include <cstdint>

#include "camera.hpp"
#include "game_object.hpp"
#include "util/fixedstring.hpp"

#include <sprite/sprite.hpp>
#include <gl/shader.hpp>
#include <gl/vao.hpp>
#include <gl/vbo.hpp>
#include <gl/texture.hpp>
#include <glad/glad.hpp>
#include <scene/scene.hpp>

extern double delta_time;
extern const char* entity_vs;
extern const char* entity_fs;
extern std::initializer_list<float> entity_vertices;


struct Entity : GameObject {
    void update() {
        render();
    }
    void render() {
        glm::mat4 mvp = current_scene->camera.getMatrix() * transform.getMatrix();
        current_shader->uniform("u_MVP", mvp);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
};

template<typename T, typename Tname>
struct EntityPool {
    SmartVector<std::shared_ptr<T>, true> entities;
    using texture_name = Tname;
    Shader shader { entity_vs, entity_fs };
    Texture* texture;
    VAO vao;
    VBO vbo {
        shader.setAttribute({"a_vert", "a_tex_coord"}, entity_vertices)
    };

    void update() {
        shader.use();
        vao.use();
        texture->use();
        // Update entities
        for (auto& e : entities) {
            e->update();
        }
    }

    template<typename... Ts>
    auto create(Ts&&... args) {
        auto e = std::make_shared<T>(std::forward<Ts>(args)...);
        entities.emplace_back(e);
        return e;
    }
};


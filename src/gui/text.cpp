#include "text.hpp"
#include <gl/texture.hpp>
#include <gl/shader.hpp>
#include <glad/glad.hpp>


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

    uint32_t i {};
    for (uint8_t c : text) {
        glm::vec2 texture_size = (glm::vec2)current_texture->size;
        current_shader->uniform("u_tex_size", (glm::vec2)char_size/texture_size);

        uint32_t rows = current_texture->size.x/char_size.x;
        uint32_t px = c%rows*char_size.x;
        uint32_t py = c/rows*char_size.y;
        current_shader->uniform("u_tex_offset", glm::vec2(glm::vec2(px, py)/texture_size));
        current_shader->uniform("u_pos", pos);
        current_shader->uniform("u_offset", glm::vec2(i, 0));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        i++;
    }
}

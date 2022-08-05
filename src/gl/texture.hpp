#pragma once
#include <cstdint>
#include <util/fixedstring.hpp>
#include <memory>
#include <string>
#include <glm/vec2.hpp>

struct Texture {
    uint32_t id;
    glm::uvec2 size;

    Texture();
    Texture(std::string_view image);
    Texture(uint32_t texture);

    Texture& operator=(Texture& other);
    Texture& operator=(std::string_view name);

    void create();
    void bindImage(std::string_view);
    void destroy();

    void use();
    void unbind();
    ~Texture();
};

extern std::string imagepath;
extern Texture* current_texture;

template<FixedString s>
struct TextureEntry {
    static constexpr FixedString str { s };
    Texture texture { s };
};

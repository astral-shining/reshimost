#pragma once
#include <cstdint>

struct VAO {
    uint32_t id;
    VAO();
    VAO(uint32_t id);

    void create();
    void destroy();
    void use();
    void unbind();
    ~VAO();
};

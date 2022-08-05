#pragma once
#include <cstdint>
#include <initializer_list>

struct EBO {
    uint32_t vbo;
    EBO();
    void bind();
    void unbind();
    ~EBO();

    void bufferDataStatic(std::initializer_list<uint32_t>);
    void bufferDataDynamic(std::initializer_list<uint32_t>);
};

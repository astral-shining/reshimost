#pragma once
#include "transform.hpp"

struct GameObject {
    Transform transform;
    uint32_t index;

    virtual void update() = 0;
    virtual void render() = 0;

    void destroy();
};


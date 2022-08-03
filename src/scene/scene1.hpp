#pragma once
#include <memory>
#include <gui/text.hpp>
#include <object/pacman.hpp>

#include "scene.hpp"

struct Scene1 : Scene<Pacman, Text> {
    std::shared_ptr<Pacman> current { create<Pacman>() };
    std::shared_ptr<Text> text { create<Text>("") };
    Scene1();
    void update();
};

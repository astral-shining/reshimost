#include "scene1.hpp"
#include <object/pacman.hpp>
#include <control/input.hpp>
//#include <object/ghost.hpp>

Scene1::Scene1() {

    //current = createObject<Pacman>();
}

void Scene1::update() {
    current->move();
    if (input.getKey(MOUSE_LEFT)) {
        auto ptr = create<Pacman>();
        ptr->transform = current->transform;
        current = ptr;
    }

    text->text = std::string("FPS:") + std::to_string(fps) + "\n" +
        "Pacmans: " + std::to_string(getManager<Pacman>().entities.size()) + "\n\n" + 
        "   Player Info\n\n" +
        "Position: " + glm::to_string((glm::vec2)current->transform.position) + "\n" +
        "Scale: " + glm::to_string((glm::vec2)current->transform.scale); 
    /*
    if (input.getKey(MOUSE_RIGHT)) {
        if (current_scene->objects.size() > 1) {
            current_scene->objects.pop_back();
            current = std::static_pointer_cast<Pacman>(current_scene->objects.back());
        }
    }

    if (auto c = current.lock()) {
        c->move();
    }*/
}

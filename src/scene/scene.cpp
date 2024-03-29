#include "scene.hpp"

#include <gui/window.hpp>
#include <object/camera.hpp>

#include <GLFW/glfw3.h>

double delta_time {};
double current_time {};
uint32_t fps {};

SceneBase* current_scene;

SceneBase::SceneBase() {
    current_scene = this;
}

void SceneBase::run() {    
    float next_second = 1;
    static uint32_t fcount {};
    double time_last_frame {};
    while (!glfwWindowShouldClose(window.getGlfwWindowPtr())) {
        delta_time = glfwGetTime()-time_last_frame;
        current_time = glfwGetTime();
        time_last_frame = current_time;

        Shader::forEach([] (Shader* s) {
            s->use();
            s->uniform("u_time", (float) current_time);
            s->uniform("u_res", (glm::vec2)window.size);
        });

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        updateScene();

        /* Swap front and back buffers */
        glfwSwapBuffers(window.getGlfwWindowPtr());

        /* Poll for and process events */
        glfwPollEvents();

        if (double t = glfwGetTime(); t > next_second) {
            fps = fcount;
            next_second = t+1;
            fcount = 0;
        }
        fcount++;
    }
}

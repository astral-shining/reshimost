#pragma once 
#include <string_view>
#include <stdint.h>
#include <memory>
#include <glm/vec2.hpp>

class GLFWwindow;

class Window {
    GLFWwindow* window;
    bool fullScreen;
    
public:
    std::string_view title;
    glm::uvec2 size;
    Window(const std::string_view title="None", glm::uvec2 size = {640, 480});
    void setSize(int w, int h);
    void setPos(int w, int h);
    void onResize(int w, int h);
    GLFWwindow* getGlfwWindowPtr() const;
    void toggleFullScreen();
    ~Window();
};

extern Window window;

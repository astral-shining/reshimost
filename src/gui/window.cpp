#include <gui/window.hpp>
#include <util/terminate.hpp>
#include <GLFW/glfw3.h>
#include <control/input.hpp>

//auto window = std::make_unique<Window>("hakugame");

Window window {"haku"};

Window::Window(const std::string_view title, glm::uvec2 size) : title(title), size(size) {
    if (!glfwInit()) {
        terminate("ERROR: glfwInit() in file ", __FILE__);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);


    window = glfwCreateWindow(size.x, size.y, title.data(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        terminate("ERROR: creating window");
    }
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);

    glfwSetWindowSizeCallback(window, [] (GLFWwindow* wnd, int w, int h) {
        static_cast<Window*>(glfwGetWindowUserPointer(wnd))->onResize(w, h);
    });

    input.init();
}

void Window::setSize(int w, int h) {
    glfwGetWindowSize(window, &w, &h);
    size = glm::uvec2(w, h);
}

void Window::setPos(int x, int y) {
    glfwSetWindowPos(window, x, y);
}

void Window::onResize(int w, int h) {
    size = glm::uvec2(w, h);
    glViewport(0, 0, w, h); // Update opengl width height
}

GLFWwindow* Window::getGlfwWindowPtr() const {
    return window;
}

void Window::toggleFullScreen() {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    //glfwSetWindowMonitor(window, monitor, 0, 0, width, height, GLFW_DONT_CARE);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

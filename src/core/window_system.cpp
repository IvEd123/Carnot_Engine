#include "core/window_system.h"

WindowSystem::WindowSystem(int width, int height, std::string title)
    : width_(width),
      height_(height),
      title_(std::move(title)),
      window_(nullptr) {}

bool WindowSystem::Init() {
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    if (!window_)
        return false;

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    return true;
}

void WindowSystem::Shutdown() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    glfwTerminate();
}

GLFWwindow* WindowSystem::GetWindow() const {
    return window_;
}

int WindowSystem::GetWidth() const {
    return width_;
}

int WindowSystem::GetHeight() const {
    return height_;
}

#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowSystem {
public:
    WindowSystem(int width = 800, int height = 800, std::string title = "Win");

    bool Init();
    void Shutdown();

    GLFWwindow* GetWindow() const;
    int GetWidth() const;
    int GetHeight() const;

private:
    int width_;
    int height_;
    std::string title_;
    GLFWwindow* window_;
};

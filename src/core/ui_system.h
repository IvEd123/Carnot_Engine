#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>

class UISystem {
public:
    UISystem();

    bool Init(GLFWwindow* window);
    void Shutdown();

private:
    ImGuiContext* context_;
};

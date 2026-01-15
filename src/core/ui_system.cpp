#include "core/ui_system.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

UISystem::UISystem() : context_(nullptr) {}

bool UISystem::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    context_ = ImGui::CreateContext();
    ImGui::SetCurrentContext(context_);

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
        return false;

    if (!ImGui_ImplOpenGL3_Init("#version 330"))
        return false;

    return true;
}

void UISystem::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    if (context_) {
        ImGui::DestroyContext(context_);
        context_ = nullptr;
    }
}

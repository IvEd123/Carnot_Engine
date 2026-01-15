#define GLEW_STATIC

#include "core/application.h"

#include <iostream>
#include <math.h>
#include <stdint.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gl/GLU.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/ui_system.h"
#include "core/window_system.h"
#include "game/scene_loader.h"
#include "object/addObject.h"
#include "object/geometricObject.h"
#include "object/light_source.h"
#include "object/player.h"
#include "object/screen.h"
#include "render/render.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#define M_PI 3.1415926535897932384626433832795

Application::Application(WindowSystem& window_system, UISystem& ui_system, SceneLoader& scene_loader)
    : window_system_(window_system),
      ui_system_(ui_system),
      scene_loader_(scene_loader) {}

bool Application::Init(int argc, char* argv[]) {
    if (!window_system_.Init())
        return false;

    if (!ui_system_.Init(window_system_.GetWindow()))
        return false;

    Player& pl = Player::Get();
    pl.SetPos(glm::vec3(0, 2, 0));

    if (argc > 0 && argv) {
        std::cout << argv[0] << std::endl;
    }

    std::cout << "WASD - move,\nalt - close,\nesc - pause(doesn't work good)" << std::endl;

    pl.proj = glm::perspective(
        glm::radians(90.0f),
        static_cast<float>(window_system_.GetWidth()) / static_cast<float>(window_system_.GetHeight()),
        0.1f,
        1000.0f);

    int error = 0;

    sun_ = LightSource();
    sun_.setShader(GL_VERTEX_SHADER, ".\\Debug\\resources\\shaders\\Shadow.vs");
    sun_.setShader(GL_FRAGMENT_SHADER, ".\\Debug\\resources\\shaders\\Shadow.fs");
    error = sun_.CreateShaders();
    if (error != 0)
        std::cout << error << std::endl;
    sun_.SetDistance(30);
    sun_.SetDir(glm::vec3(1, -1, 1));

    screen_ = Screen();
    screen_.frameBuffer = createFrameBuffer(
        window_system_.GetWidth(),
        window_system_.GetHeight(),
        screen_.getDepthSteencilBuffer(),
        screen_.getColorBuffer());
    screen_.material.loadShader(GL_VERTEX_SHADER, ".\\Debug\\resources\\shaders\\screen.vs");
    screen_.material.loadShader(GL_FRAGMENT_SHADER, ".\\Debug\\resources\\shaders\\screen.fs");
    error = screen_.material.CreateShaders();
    screen_.material.specifyVertexAttributes_screen(screen_.material.getShaderProgram());
    if (error != 0)
        std::cout << error << std::endl;
    screen_.addLightSource(&sun_);

    scene_loader_.LoadScene();
    scene_loader_.AttachLightSources(sun_);
    scene_loader_.StartScripts();

    return true;
}

int Application::Run(int argc, char* argv[]) {
    if (!Init(argc, argv))
        return -1;

    bool pause = false;

    GLFWwindow* window = window_system_.GetWindow();
    Player& pl = Player::Get();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (!pause) {
#ifdef _WIN32
            POINT mousexy;
            GetCursorPos(&mousexy);
            glm::ivec2 center = {
                window_system_.GetWidth() / 2,
                window_system_.GetHeight() / 2
            };

            pl.SetAng({
                (int)(pl.GetAng().x + (center.x - mousexy.x) / 4) % 360,
                pl.GetAng().y + (center.y - mousexy.y) / 3
            });

            if (pl.GetAng().y < -89)
                pl.SetAng({ pl.GetAng().x, -89 });
            if (pl.GetAng().y > 89)
                pl.SetAng({ pl.GetAng().x, 89 });
            SetCursorPos(center.x, center.y);
#endif
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pl.view = glm::lookAt(
            pl.GetPos(),
            glm::vec3(pl.GetPos().x - sin(pl.GetAng().x / 180 * M_PI),
                      pl.GetPos().y + tan(pl.GetAng().y / 180 * M_PI),
                      pl.GetPos().z - cos(pl.GetAng().x / 180 * M_PI)),
            glm::vec3(0.0f, 1.0f, 0.0f));

        pl.Move(pl.GetAng(), 0.01);

        glCullFace(GL_FRONT);
        sun_.Draw(obj_list);
        glCullFace(GL_BACK);
        glViewport(0, 0, window_system_.GetWidth(), window_system_.GetHeight());
        glBindFramebuffer(GL_FRAMEBUFFER, screen_.frameBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        for (int i = 0; i < obj_list.size(); i++) {
            obj_list[i]->Draw();
        }

        scene_loader_.UpdateScripts();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        screen_.material.attachUniform("time", 0.01f);

        screen_.view = &pl.view;
        screen_.proj = &pl.proj;
        screen_.addLightSource(&sun_);
        screen_.Draw();

        glfwSwapBuffers(window);
    }

    Shutdown();
    return 0;
}

void Application::Shutdown() {
    ui_system_.Shutdown();
    window_system_.Shutdown();
}

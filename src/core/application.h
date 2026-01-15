#pragma once

#include "object/light_source.h"
#include "object/screen.h"

class UISystem;
class WindowSystem;
class SceneLoader;

class Application {
public:
    Application(WindowSystem& window_system, UISystem& ui_system, SceneLoader& scene_loader);

    int Run(int argc, char* argv[]);
    bool Init(int argc, char* argv[]);
    void Shutdown();

private:
    WindowSystem& window_system_;
    UISystem& ui_system_;
    SceneLoader& scene_loader_;
    LightSource sun_;
    Screen screen_;
};

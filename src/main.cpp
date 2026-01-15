#include "core/application.h"
#include "core/ui_system.h"
#include "core/window_system.h"
#include "game/scene_loader.h"

int main(int argc, char* argv[]) {
    WindowSystem window_system;
    UISystem ui_system;
    SceneLoader scene_loader;
    Application application(window_system, ui_system, scene_loader);
    return application.Run(argc, argv);
}

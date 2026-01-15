#include "game/scene_loader.h"

#include <iostream>

#include <glm/glm.hpp>

#include "object/geometricObject.h"
#include "object/light_source.h"
#include "object/saveLoad.h"
#include "script/dllScriptHandler.h"

std::vector<GeometricObject*> obj_list;
std::vector<LightSource*> light_list;
std::vector<std::vector<glm::vec3>> array_of_vertecies;
std::vector<std::vector<glm::vec3>> array_of_normals;
std::vector<std::vector<glm::vec2>> array_of_uvs;
std::vector<DLLScriptHandler> scripts;

bool SceneLoader::LoadScene() {
    SaveLoad sf = SaveLoad();
    std::cout << "Enter path to save file:\n";
    std::string path_to_safe_file;
    std::cin >> path_to_safe_file;
    sf.SetPath(path_to_safe_file);
    std::string name;
    std::cout << "Enter name of save file (without extension):\n";
    std::cin >> name;
    sf.SetName(name);

    sf.Load();
    return true;
}

void SceneLoader::AttachLightSources(LightSource& light_source) {
    for (int i = 0; i < obj_list.size(); i++)
        obj_list[i]->addLightSource(&light_source);
}

void SceneLoader::StartScripts() {
    for (int i = 0; i < scripts.size(); i++)
        scripts[i].Start();
}

void SceneLoader::UpdateScripts() {
    for (int i = 0; i < scripts.size(); i++)
        scripts[i].Update();
}

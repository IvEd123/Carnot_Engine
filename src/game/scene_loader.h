#pragma once

#include <vector>

class LightSource;
class DLLScriptHandler;
class GeometricObject;

class SceneLoader {
public:
    bool LoadScene();
    void AttachLightSources(LightSource& light_source);
    void StartScripts();
    void UpdateScripts();
};

extern std::vector<GeometricObject*> obj_list;
extern std::vector<LightSource*> light_list;
extern std::vector<DLLScriptHandler> scripts;

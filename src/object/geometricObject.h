#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <vector>
#include <filesystem>
#include <GL/glew.h>
#include "../material/material.h"

class LightSource;
class DLLScriptHandler;


class GeometricObject {
private:

    std::string name;
protected:

    int type;
    glm::vec3 size;
    glm::vec3 pos;
    glm::vec3 rot;


    void deleteArrays();
public:
    GeometricObject() {}
    ~GeometricObject() {};
    virtual void Delete() = 0;
    void decrementIndex();


    void SetType(int t);
    int GetType();
    void SetName(std::string);
    std::string GetName(void);
    std::string* GetNamePtr(void);


    glm::vec3 GetPos();
    void SetPos(glm::vec3 _pos);
    glm::vec3 GetRot();
    void SetRot(glm::vec3 _rot);
    void SetSize(glm::vec3 s);
    glm::vec3 GetSize();


    glm::vec3* GetPosPtr();
    glm::vec3* GetRotPtr();
    glm::vec3* GetSizePtr();

    Material material;
    bool cast_shadow = true;
    void UpdateModelMatrix();
    void addLightSource(LightSource* source);
    virtual void Draw() = 0;
    virtual void setModel(std::filesystem::path path) = 0;

    std::vector<GLfloat> vertices;
    std::vector<std::vector<glm::vec3>>* vert_vec3;
    std::vector<std::vector<glm::vec2>>* uv_vec2;
    std::vector<std::vector<glm::vec3>>* norm_vec3;
    int array_index;
    virtual void CreateVertices() = 0;
};
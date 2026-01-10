#pragma once

#include "geometricObject.h"


class LightSource {
private:
                            
    std::string                             name;
    glm::vec3                            pos;
    glm::vec3                            rot;
    glm::vec3                            dir;
    std::string                             vertexShader_source;
    std::string                             fragmentShader_source;
    int                                     resolution;
    unsigned int                            depthMapFBO;
    unsigned int                            depthMap;
    unsigned int                            ShaderProgram;
    float                                   fov = 20;
    float                                   distance;

    glm::vec3                            pov;

    int                                     loadShader(GLenum type, const GLchar* path);

    float                                   near_plane, 
                                            far_plane;
    glm::mat4                               lightProjection, lightView, lightSpaceMatrix;
    void                                    updatePos();

    int                                     CreateShaderProgram();
public:

    LightSource();
    void                                    Delete();
    void                                    setShader(GLenum type, const GLchar* path);
    int                                     CreateShaders();
    void                                    SetName(std::string);
    std::string                             GetName(void);

    glm::mat4 *                             getProjMatrix() { return &lightSpaceMatrix; }

    unsigned int *                          getShadowMap();

    glm::vec3                            GetPos() { return pos; }
    glm::vec3  *                          GetPosPtr() { return &pos; }
    void                                    SetPos(glm::vec3 _pos) { pos = _pos; }
    glm::vec3                            GetPov() { return pov; }
    void                                    SetPov(glm::vec3 _pov) { pov = _pov; }
    glm::vec3                            GetRot() { return rot; }
    glm::vec3  *                         GetRotPtr() { return &rot; }
    void                                    SetRot(glm::vec3 _rot) { rot = _rot; }
    void                                    SetDistance(float _distance) { distance = _distance; }
    float                                   GetDistance() { return distance; }
    glm::vec3                            GetDir() { return dir; }
    glm::vec3   *                        GetDirPtr() { return &dir; }
    void                                    SetDir();
    void                                    SetDir(glm::vec3);

    void                                    Draw(std::vector <GeometricObject*> obj_list);
};

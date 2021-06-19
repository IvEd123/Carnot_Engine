#pragma once
#ifndef GEOMOBJ_H
#define GEOMOBJ_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>

#include "../thirdparty/glew-2.1.0/include/GL/glew.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GLU.h>

#include "../Headers/PLayer.h"
#include "../Headers/Render.h"
#include "../Headers/DLLScriptHandler.h"

void getError(std::vector<GLchar>log, GLuint shader);

class LightSource;

class GeometricObject{
private:
                                            
    std::string                             name;
protected:
    
    int                                     type;
    float                                   size;
    sf::Vector3f                            pos;
    sf::Vector3f                            rot;
    
public:
    GeometricObject(){}
    ~GeometricObject();

    void                                    SetType(int t);
    int                                     GetType();
    void                                    SetName(std::string);
    std::string                             GetName(void);


    sf::Vector3f                            GetPos();
    void                                    SetPos(sf::Vector3f _pos);
    sf::Vector3f                            GetRot();
    void                                    SetRot(sf::Vector3f _rot);
    void                                    SetSize(float s);
    float                                   GetSize();
    
    sf::Vector3f *                          GetPosPtr();
    sf::Vector3f *                          GetRotPtr();
    float        *                          GetSizePtr();

    DLLScriptHandler script;

    Material                                material;
    bool                                    cast_shadow = true;
    void                                    UpdateModelMatrix();
    void                                    addLightSource(LightSource* source);
    virtual void                            Draw() = 0;
    virtual void                            setModel(char* path) = 0;

    std::vector<GLfloat>                    vertices;
    std::vector<std::vector<glm::vec3>>*    vert_vec3;
    std::vector<std::vector<glm::vec2>>*    uv_vec2;
    std::vector<std::vector<glm::vec3>>*    norm_vec3;
    int                                     array_index;
    virtual void                            CreateVertices() = 0;
};


class Cube : public GeometricObject {
private:
    
    
public:
    Cube(float _size);
    Cube(sf::Vector3f _pos, sf::Vector3f _rot, float _size, GLuint* _texture);
    Cube();
    ~Cube();

    void                                    Draw();
    void                                    CreateVertices();
    void                                    setModel(char* path) {}

};

class Screen : public GeometricObject {
private:
    GLuint                                  depth_stencil_buff;
    GLuint                                  tex;
public:
    GLuint                                  frameBuffer;
    GLuint *                                getDepthSteencilBuffer();
    GLuint *                                getColorBuffer();
    void                                    setColorBuffer(GLuint t) { tex = t; }

    glm::mat4 *                             view; 
    glm::mat4 *                             proj;
    void                                    CreateVertices();
    void                                    Draw();
    void                                    setModel(char* path) {}
    Screen();
    ~Screen();
};

class Terrain : public GeometricObject{
public:
    Terrain(float _size, float _res, float _height);
    ~Terrain();
    Terrain();

    void                                    Draw();
    void                                    CreateVertices();

    GLuint *                                getHeightmap() { return &heightmap; }
    GLuint *                                getAOMap() { return &ao; }
    GLuint *                                getNormalMap() { return &norm; }
    void                                    setModel(char* path) {}
    float                                   height;
private:
    float                                   size;
    int                                     resolution;

    GLuint                                  heightmap;
    GLuint                                  ao;
    GLuint                                  norm;
};



class  Mesh : public GeometricObject {
public:
    std::string                             model_path;
    Mesh(char* path);
    void                                    setModel( char* path);
    Mesh();
    ~Mesh();
    void                                    Draw();
    void                                    CreateVertices();
};


class Plane : public GeometricObject {
public:
    
    Plane();
    ~Plane();
    void                                    Draw();
    void                                    CreateVertices();
    void                                    setModel(char* path) {}
    int                                     texture_repeat = 1;

    GLuint *                                getNormalMap() { return &norm; }
    GLuint *                                getSpecMap() { return &spec; }
private:
    GLuint                                  norm;
    GLuint                                  spec;
};



//LIGHT


class LightSource {
private:
    std::string                             name;
    sf::Vector3f                            pos;
    sf::Vector3f                            rot;
    std::string                             vertexShader_source;
    std::string                             fragmentShader_source;
    int                                     resolution;
    unsigned int                            depthMapFBO;
    unsigned int                            depthMap;
    unsigned int                            ShaderProgram;
    float                                   fov = 20;
    sf::Vector3f                            pov;

    int                                     loadShader(GLenum type, const GLchar* path);

    float                                   near_plane, 
                                            far_plane;
    glm::mat4                               lightProjection, lightView, lightSpaceMatrix;
    
    int                                     CreateShaderProgram();
public:

    LightSource();
    void                                    setShader(GLenum type, const GLchar* path);
    int                                     CreateShaders();
    void                                    SetName(std::string);
    std::string                             GetName(void);

    glm::mat4 *                             getProjMatrix() { return &lightSpaceMatrix; }

    unsigned int *                          getShadowMap();

    sf::Vector3f                            GetPos() { return pos; }
    void                                    SetPos(sf::Vector3f _pos) { pos = _pos; }
    sf::Vector3f                            GetPov() { return pov; }
    void                                    SetPov(sf::Vector3f _pov) { pov = _pov; }
    sf::Vector3f                            GetRot() { return rot; }
    void                                    SetRot(sf::Vector3f _rot) { rot = _rot; }

    void                                    Draw(std::vector <GeometricObject*> obj_list);
};


#endif
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
class DLLScriptHandler;

class GeometricObject{
private:
                                            
    std::string                             name;
protected:
    
    int                                     type;
    sf::Vector3f                            size;
    sf::Vector3f                            pos;
    sf::Vector3f                            rot;
    
public:
    GeometricObject(){}
    ~GeometricObject();

    void                                    SetType(int t);
    int                                     GetType();
    void                                    SetName(std::string);
    std::string                             GetName(void);
    std::string *                           GetNamePtr(void);


    sf::Vector3f                            GetPos();
    void                                    SetPos(sf::Vector3f _pos);
    sf::Vector3f                            GetRot();
    void                                    SetRot(sf::Vector3f _rot);
    void                                    SetSize(sf::Vector3f s);
    sf::Vector3f                            GetSize();
    
    sf::Vector3f *                          GetPosPtr();
    sf::Vector3f *                          GetRotPtr();
    sf::Vector3f *                          GetSizePtr();

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
    std::string                             model_path;
    
public:
    Cube(sf::Vector3f _size);
    Cube(sf::Vector3f _pos, sf::Vector3f _rot, sf::Vector3f _size, GLuint* _texture);
    Cube();
    ~Cube();

    void                                    Draw();
    void                                    Draw(sf::Vector3f);
    void                                    CreateVertices() {};
    void                                    CreateVerticesLegacy();
    void                                    setModel(char* path);

    sf::Vector3f size_v;

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


class Cloudbox : public Cube {
public:
    Cloudbox(sf::Vector3f _pos, sf::Vector3f _res, sf::Vector3f size);
    void                                    RenderCloud(float innerRadius, float outerRadius, sf::Vector3f center);
    void                                    renderTexture(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    struct                                  CloudParams {
        glm::vec4                           phaseParams = glm::vec4(0.72, 0.33, 1, 0.74);

        glm::vec3
                                            LightColor = glm::vec3(1, 1, 1),
                                            cloudScale = glm::vec3(10),
                                            cloudOffset = glm::vec3(0),
                                            secondLayerScale = glm::vec3(10),
                                            secondLayerOffset = glm::vec3(0),
                                            thirdLayerScale = glm::vec3(10),
                                            thirdLayerOffset = glm::vec3(0);

        float
                                            DensityThreshold = 0.3,
                                            DensityMultiplier = 50,
                                            lightAbsorptionThroughCloud = 0.85,
                                            lightAbsorptionTowardSun = 2.0,
                                            darknessThreshold = 0.2;

        int
                                            num_of_steps = 50,
                                            num_of_steps_inside = 25;
    };
    CloudParams                             cloudParams;
    void                                    recreateShaders();
    void                                    uniforms();
    GLuint                                  GetTexture() { return cloudtex; }
private:
    sf::Vector3f                            cloudTexRes;
    const int                               pointsGrid = 7;
    GLuint                                  cloudbuffer;
    GLuint                                  cloudtex;

    void                                    initBuffer();
    void                                    initTexture();
    void                                    attachBuffer();
    
    
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
    sf::Vector3f  *                          GetPosPtr() { return &pos; }
    void                                    SetPos(sf::Vector3f _pos) { pos = _pos; }
    sf::Vector3f                            GetPov() { return pov; }
    void                                    SetPov(sf::Vector3f _pov) { pov = _pov; }
    sf::Vector3f                            GetRot() { return rot; }
    void                                    SetRot(sf::Vector3f _rot) { rot = _rot; }

    void                                    Draw(std::vector <GeometricObject*> obj_list);
};


class Sky {
public:
    Sky(Cloudbox*);
    void                                    Render();
    GLuint                                  GetTex();
    float                                   angle = 0.5;  
    sf::Vector3f                            centerPos;
    float                                   innerRadius;
    float                                   outerRadius;
    void                                    initSky(const std::string&, GeometricObject*);
protected:
    struct SkySphere {
        Mesh*                               sphereMesh;
        GLuint                              texture;

        GLuint                              fragmentShader;
        GLuint                              vertexShader;
        GLuint                              shaderProgram;

        void                                attachShader(const std::string&);
        void                                createShaderProgram();
        void                                initTexture(int res);
    };
    struct Cloud {
        Cloudbox*                           cloudbox;
        GLuint                              cloudCubeMap;
        
        
        void                                initTexture(int res);
    };
    struct Camera {
        sf::Vector3f                        cameraPos = sf::Vector3f(0, 0, 0);
        float                               pitch = 0;
        float                               yaw = 0;
        glm::mat4                           proj; 
        glm::mat4                           view;
        
        void                                createMatrices();
        
        void                                switchToFace(int faceIndex);
    };

    LightSource*                            sun;
    struct Cloud                            cloudsOnSky;
    struct Camera                           camera;
    struct SkySphere                        skySphere;
         
    GLuint                                  skyBoxTexture;
    GLuint                                  skyBoxFrameBuffer;
    GLuint buff;
    int                                     cubemapRes = 128;

    void                                    attachMeshToSky(GeometricObject*);
    void                                    RenderCloud();
    void                                    RenderSky();
    void                                    updateMatrices();
    void                                    initFramebuffer();
    void                                    initTexture();
    void                                    setRadius();
    void                                    setCloudBoxPosition();
};

#endif
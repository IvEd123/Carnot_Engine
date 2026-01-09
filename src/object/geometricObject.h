#pragma once
#ifndef GEOMOBJ_H
#define GEOMOBJ_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>

#include <GL/glew.h>
#include <gl/GLU.h>


#include "player.h"
#include "../render/render.h"
#include "../script/dllScriptHandler.h"

void getError(std::vector<GLchar>log, GLuint shader);

class LightSource;
class DLLScriptHandler;

class GeometricObject{
private:
                                            
    std::string                             name;
protected:
    
    int                                     type;
    glm::vec3                            size;
    glm::vec3                            pos;
    glm::vec3                            rot;
    

    void                                    deleteArrays();
public:
    GeometricObject()   {}
    ~GeometricObject() {}   ;
    virtual void                            Delete() = 0;
    void                                    decrementIndex();
    

    void                                    SetType(int t);
    int                                     GetType();
    void                                    SetName(std::string);
    std::string                             GetName(void);
    std::string *                           GetNamePtr(void);


    glm::vec3                            GetPos();
    void                                    SetPos(glm::vec3 _pos);
    glm::vec3                            GetRot();
    void                                    SetRot(glm::vec3 _rot);
    void                                    SetSize(glm::vec3 s);
    glm::vec3                            GetSize();

    
    glm::vec3 *                          GetPosPtr();
    glm::vec3 *                          GetRotPtr();
    glm::vec3 *                          GetSizePtr();

    Material                                material;
    bool                                    cast_shadow = true;
    void                                    UpdateModelMatrix();
    void                                    addLightSource(LightSource* source);
    virtual void                            Draw() = 0;
    virtual void                            setModel(std::string& path) = 0;

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
    Cube(glm::vec3 _size);
    Cube(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _size, GLuint* _texture);
    Cube();
    ~Cube();

    void                                    Draw();
    void                                    Draw(glm::vec3);
    void                                    Delete();
    void                                    CreateVertices() {};
    void                                    CreateVerticesLegacy();
    void                                    setModel(std::string& path);

    glm::vec3 size_v;

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
    void                                    setModel(std::string& path) {}
    void                                    Delete();
    Screen();
    ~Screen();
};

class Terrain : public GeometricObject{
public:
    Terrain(float _size, float _res, float _height);
    ~Terrain();
    Terrain();

    void                                    Delete() {};

    void                                    Draw();
    void                                    CreateVertices();

    GLuint *                                getHeightmap() { return &heightmap; }
    GLuint *                                getAOMap() { return &ao; }
    GLuint *                                getNormalMap() { return &norm; }
    void                                    setModel(std::string& path) {}
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
    Mesh(std::string& path);
    void                                    Delete();
    void                                    setModel(std::string& path);
    Mesh();
    ~Mesh();
    void                                    Draw();
    void                                    CreateVertices();
};


class Plane : public GeometricObject {
public:
    
    Plane();
    ~Plane();
    void                                    Delete();
    void                                    Draw();
    void                                    CreateVertices();
    void                                    setModel(std::string& path) {}
    int                                     texture_repeat = 1;

    GLuint *                                getNormalMap() { return &norm; }
    GLuint *                                getSpecMap() { return &spec; }
private:
    GLuint                                  norm;
    GLuint                                  spec;
};


class Cloudbox : public Cube {
public:
    Cloudbox(glm::vec3 _pos, glm::vec3 _res, glm::vec3 size);
    void                                    Delete();
    void                                    RenderCloud(float innerRadius, float outerRadius, glm::vec3 center);
    void                                    renderTexture(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    struct                                  CloudParams {
        glm::vec4                           phaseParams = glm::vec4(0.72, 0.33, 1, 0.74);

        glm::vec3
                                            LightColor = glm::vec3(1, 1, 1),
                                            cloudScale = glm::vec3(1),
                                            cloudOffset = glm::vec3(0),
                                            secondLayerScale = glm::vec3(2),
                                            secondLayerOffset = glm::vec3(0),
                                            thirdLayerScale = glm::vec3(3),
                                            thirdLayerOffset = glm::vec3(0);

        float
                                            DensityThreshold = 0.93,
                                            DensityMultiplier = 72,
                                            lightAbsorptionThroughCloud = 0.85,
                                            lightAbsorptionTowardSun = 2.0,
                                            darknessThreshold = 0.2;

        int
                                            num_of_steps = 80,
                                            num_of_steps_inside = 50;
    };
    CloudParams                             cloudParams;
    void                                    recreateShaders();
    void                                    uniforms();
    GLuint                                  GetTexture() { return cloudtex; }
private:
    glm::vec3                            cloudTexRes;
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


class Sky {
public:
    Sky(Cloudbox*);
    void                                    Delete();
    void                                    Render(int i);
    GLuint                                  GetTex();
    float                                   angle = 0.5;  
    glm::vec3                            centerPos;
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
        glm::vec3                        cameraPos = glm::vec3(0, 0, 0);
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
    int                                     cubemapRes = 512;

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
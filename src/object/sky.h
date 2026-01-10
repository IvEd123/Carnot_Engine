#pragma once
#include "cloud_box.h"
#include "mesh.h"

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
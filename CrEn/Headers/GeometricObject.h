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
                                            
    std::string                             m_name;
protected:
    
    int                                     m_type;
    sf::Vector3f                            m_size;
    sf::Vector3f                            m_position;
    sf::Vector3f                            m_rotation;
    
    

    void                                    deleteArrays();
public:
     GeometricObject() {}
    ~GeometricObject() {}

    void                                    DecrementIndex();
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
    void                                    AddLightSource(LightSource* source);
    virtual void                            Draw() = 0;
    virtual void                            SetModel(std::string& path) = 0;

    int                                     m_texture_repeat = 1;
    std::vector<GLfloat>                    vertices;
    std::vector<std::vector<glm::vec3>>*    vert_vec3;
    std::vector<std::vector<glm::vec2>>*    uv_vec2;
    std::vector<std::vector<glm::vec3>>*    norm_vec3;
    int                                     array_index;
    virtual void                            CreateVertices() = 0;
};


class Cube : public GeometricObject {
private:
    std::string                             m_model_path;
    
public:
    Cube(sf::Vector3f _size);
    Cube(sf::Vector3f _pos, sf::Vector3f _rot, sf::Vector3f _size, GLuint _texture);
    Cube();
    ~Cube();

    void                                    Draw();
    void                                    Draw(sf::Vector3f);
    void                                    CreateVertices() {};
    void                                    CreateVerticesLegacy();
    void                                    SetModel(std::string& path);

    sf::Vector3f size_v;

};

class Screen : public GeometricObject {
private:
    GLuint                                  m_color_texture;
    GLuint                                  m_albedo_texture;
    GLuint                                  m_pos_texture;
    GLuint                                  m_norm_texture;
    unsigned int                            m_depth_texture;
public:
    GLuint                                  frameBuffer;
    GLuint *                                GetColorBuffer();
    void                                    SetColorBuffer(GLuint t) { m_color_texture = t; }
    GLuint*                                 GetDepthTex() { return &m_depth_texture; }
    GLuint*                                 GetPosTex() { return &m_pos_texture; }
    GLuint*                                 GetNormTex() { return &m_norm_texture; }
    GLuint*                                 GetAlbedoTex() { return &m_albedo_texture; }
    glm::mat4 *                             view_matrix; 
    glm::mat4 *                             projection_matrix;
    void                                    CreateVertices();
    void                                    Draw();
    void                                    SetModel(std::string& path) {}
    void                                    CreateFrameBuffer(int height, int width);
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

    GLuint *                                getHeightmap() { return &m_height_texture; }
    GLuint *                                GetAmbientOcclusionMap() { return &m_ambient_occlusion_texture; }
    GLuint *                                GetNormalMap() { return &m_normal_texture; }
    void                                    SetModel(std::string& path) {}
    float                                   height;
private:
    float                                   m_size;
    int                                     m_resolution;

    GLuint                                  m_height_texture;
    GLuint                                  m_ambient_occlusion_texture;
    GLuint                                  m_normal_texture;
};



class  Mesh : public GeometricObject {
public:
    std::string                             model_path;
    Mesh(std::string& path);
    void                                    SetModel(std::string& path);
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
    void                                    SetModel(std::string& path) {}


};


class Cloudbox : public Cube {
public:
    Cloudbox(sf::Vector3f _pos, sf::Vector3f _res, sf::Vector3f size);
    ~Cloudbox();
    void                                    RenderCloud(float innerRadius, float outerRadius, sf::Vector3f center);
    void                                    RenderTexture(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    struct                                  CloudParams {
        glm::vec4                           m_phase_params = glm::vec4(0.72, 0.33, 1, 0.74);

        glm::vec3
                                            m_light_color = glm::vec3(1, 1, 1),
                                            m_cloud_scale = glm::vec3(1),
                                            m_cloud_offset = glm::vec3(0),
                                            m_second_layer_scale = glm::vec3(2),
                                            m_second_layer_offset = glm::vec3(0),
                                            m_third_layer_scale = glm::vec3(3),
                                            m_third_layer_offset = glm::vec3(0);

        float
                                            m_density_threshold = 0.93,
                                            m_density_multiplier = 72,
                                            m_light_absorption_through_cloud = 0.85,
                                            m_light_absorption_toward_light = 2.0,
                                            m_darkness_threshold = 0.2;

        int
                                            m_num_of_steps_shape = 80,
                                            m_num_of_steps_light = 50;
    };
    CloudParams                             cloudParams;
    void                                    ChangeShaders();
    void                                    AttachUniforms();
    GLuint                                  GetTexture() { return m_cloud_texture; }
private:
    sf::Vector3f                            m_cloud_texure_resolution;
    const int                               m_points_in_grid = 7;
    GLuint                                  m_cloudbuffer;
    GLuint                                  m_cloud_texture;

    void                                    initBuffer();
    void                                    initTexture();
    void                                    attachBuffer();
    
    
};


//LIGHT


class LightSource {
private:
                            
    std::string                             m_name;
    sf::Vector3f                            m_position;
    sf::Vector3f                            m_rotation;
    sf::Vector3f                            m_direction;
    std::string                             m_vertex_shader_source;
    std::string                             m_fragment_shader_source;
    int                                     m_shadowmap_resolution;
    GLuint                                  m_depthmap_buffer;
    GLuint                                  m_depthmap;
    GLuint                                  m_shader_program;
    float                                   m_field_of_view = 20;
    float                                   m_draw_distance;



    int                                     loadShader(GLenum type, const GLchar* path);

    float                                   m_near_plane, 
                                            m_far_plane;
    glm::mat4                               m_light_projection_matrix, m_light_view_matrix, m_light_space_matrix;
    void                                    updatePos();

    int                                     CreateShaderProgram();
public:

     LightSource();
    ~LightSource();
    void                                    SetShader(GLenum type, const GLchar* path);
    int                                     CreateShaders();
    void                                    SetName(std::string);
    std::string                             GetName(void);

    glm::mat4 *                             GetProjMatrix() { return &m_light_space_matrix; }

    unsigned int *                          GetShadowMap();

    sf::Vector3f                            GetPos() { return m_position; }
    sf::Vector3f  *                         GetPosPtr() { return &m_position; }
    void                                    SetPos(sf::Vector3f _pos) { m_position = _pos; }
    sf::Vector3f                            GetRot() { return m_rotation; }
    sf::Vector3f  *                         GetRotPtr() { return &m_rotation; }
    void                                    SetRot(sf::Vector3f _rot) { m_rotation = _rot; }
    void                                    SetDistance(float _distance) { m_draw_distance = _distance; }
    float                                   GetDistance() { return m_draw_distance; }
    sf::Vector3f                            GetDir() { return m_direction; }
    sf::Vector3f   *                        GetDirPtr() { return &m_direction; }
    void                                    SetDir();
    void                                    SetDir(sf::Vector3f);
    

    void                                    Draw(std::vector <GeometricObject*> obj_list);
};


class Sky {
public:
     Sky(Cloudbox*);
    ~Sky();
    void                                    Render(int i);
    GLuint                                  GetTex(); 
    const sf::Vector3f                      c_center_position = sf::Vector3f(0, -200, 0);
    const float                             c_inner_radius = 210;
    const float                             c_outer_radius = 250;
    void                                    initSky(const std::string&, GeometricObject*);
protected:
    struct SkySphere {
        Mesh*                               m_sphere_mesh;
        GLuint                              m_texture;

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
    int                                     cubemapRes = 512;

    void                                    attachMeshToSky(GeometricObject*);
    void                                    RenderCloud();
    void                                    RenderSky();
    void                                    updateMatrices();
    void                                    initFramebuffer();
    void                                    initTexture();
};

#endif
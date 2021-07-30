#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <string>
#include <vector>

#include "../thirdparty/glew-2.1.0/include/GL/glew.h"
#include <GL/glew.h>
#include "../thirdparty/glm/ext/matrix_float4x4.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/GLU.h>


class  Material {
public:
                                            //
    Material();
    ~Material();

    std::vector<GLchar>                     log_vs;
    std::vector<GLchar>                     log_gs;
    std::vector<GLchar>                     log_fs;
    std::string                             tex_path;
    
    glm::mat4 *                             lightSpaceMatrixPtr;
    unsigned int *                          shadowmap;
    glm::vec3   *                            sun_pos;

    int                                     CreateShaders();
    void                                    loadShader(GLenum type, const GLchar* path);
    void                                    bindTexture(const GLchar* path);
    void                                    bindTexture(GLuint texture);
    void                                    bindTexture(GLuint texure, const GLchar* name);
    void                                    specifyVertexAttributes(GLuint shaderProgram);
    void                                    specifyVertexAttributes3D(GLuint shaderProgram);
    void                                    specifyVertexAttributes_screen(GLuint shaderProgram);
    void                                    specifyVertexAttributes_terrain(GLuint shaderProgram);
    void                                    specifyVertexAttributes_mesh();

    void                                    createVAO_VBO(std::vector<GLfloat>vertices);
    void                                    createVAO_VBO_mesh(std::vector<glm::vec3>& vert_vec3, std::vector<glm::vec2>& uv_vec2, std::vector<glm::vec3>& norm_vec3);

    void                                    attachUniform(const char* name, float value);
    void                                    attachUniform(const char* name, int value);
    void                                    attachUniform(const char* name, glm::vec3 value);
    void                                    attachUniform(const char* name, GLuint tex);

    GLuint                                  getVAO() { return vao; }
    GLuint                                  getShaderProgram() { return shaderProgram; }
    GLuint                                  getUniModel() { return uniModel; }
    glm::mat4                               getModel() { return model; }
    GLuint                                  getTexture() { return texture; }

    void                                    setUniModel(GLuint uniform) { uniModel = uniform; }
    void                                    setModel(glm::mat4 _model) { model = _model; }
    void                                    setTexture(GLuint _texture) { texture = _texture; }

    std::string                             GetVSPath() { return vertexShader_path; }
    
    std::string                             GetFSPath() { return fragmentShader_path; }

    void                                    updateUniforms();
private:
    int                                     createShaderProgram(const GLchar* vertSrc, const GLchar* geomSrc, const GLchar* fragSrc, GLuint& vertexShader, GLuint& geometryShader, GLuint& fragmentShader, GLuint& shaderProgram);
   
    GLuint                                  vao;
    GLuint                                  vbo;
    GLuint                                  vert_buff;
    GLuint                                  norm_buff;
    GLuint                                  uv_buff;
    GLuint                                  texture;
    GLuint                                  vertexShader;
    GLuint                                  geometryShader;
    GLuint                                  fragmentShader;
    GLuint                                  shaderProgram;
    GLchar*                                 vertexShader_source;
    GLchar*                                 geometryShader_source;
    GLchar*                                 fragmentShader_source;
    std::string                             vertexShader_path;
    std::string                             geometryShader_path;
    std::string                             fragmentShader_path;
    GLuint                                  uniModel;
    glm::mat4                               model;  
};

#endif
#pragma once

#include "light_source.h"
#include <glm/glm.hpp>
#include "utils.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


LightSource::LightSource() {
    resolution = 2048;
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    near_plane = 1.0f, far_plane = 50.0f;
    lightProjection = glm::ortho(-fov, fov, -fov, fov, near_plane, far_plane);
    
}

void LightSource::setShader(GLenum type, const GLchar* path){
    switch (type){
    case GL_VERTEX_SHADER:
        vertexShader_source = std::string(path);
        break;
    case GL_FRAGMENT_SHADER:
        fragmentShader_source = std::string(path);
        break;
    default:
        break;
    }
}

int LightSource::CreateShaders(){
    return CreateShaderProgram();
}

void LightSource::Draw(std::vector <GeometricObject*> obj_list) {

    glEnable(GL_DEPTH_TEST);
    glUseProgram(ShaderProgram);
    
 
    glViewport(0, 0, resolution, resolution);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

   // SetDir();
    updatePos();

    lightView = glm::lookAt(pos, { 0, 0, 0 }, { 0.0f, 1.0f, 0.0f });

    lightSpaceMatrix = lightProjection * lightView;

    for (int i = 0; i < obj_list.size(); i++) {
       
        if (obj_list[i]->cast_shadow) {

            glm::vec3 pos = obj_list[i]->GetPos();
            glm::vec3 rot = obj_list[i]->GetRot();

            glm::mat4 model = glm::mat4(1.0);
            model = glm::translate(model, pos);
            
            model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
            model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
            model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));

            GLuint lightSpaceMatrixLocation = glGetUniformLocation(ShaderProgram, "mvpMatrix");
            glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

            GLuint model_loc = glGetUniformLocation(ShaderProgram, "model");
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

            GLuint size_loc = glGetUniformLocation(ShaderProgram, "size");
            glUniform3f(size_loc, obj_list[i]->GetSize().x, obj_list[i]->GetSize().y, obj_list[i]->GetSize().z);

            glBindVertexArray(obj_list[i]->material.getVAO());

            int size = (*(*obj_list[i]).vert_vec3)[i].size();
            glDrawArrays(GL_TRIANGLES, 0, size);
            glBindVertexArray(0);
        }
    }

}

unsigned int *LightSource::getShadowMap() {
    return &depthMap;
}

int LightSource::CreateShaderProgram() {

    const char* vs_str = vertexShader_source.c_str();
    const char* fs_str = fragmentShader_source.c_str();

    int vertexShader = loadShader(GL_VERTEX_SHADER, vs_str);
    if (vertexShader == -1)
        return -1;
    
    int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fs_str);
    if (fragmentShader == -1)
        return -1;

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, vertexShader);
    glAttachShader(ShaderProgram, fragmentShader);

    glLinkProgram(ShaderProgram);
}

 int LightSource::loadShader(GLenum type, const GLchar* path) {

    std::ifstream file;
    file.open(path);

    if (!file.is_open()) {
        std::cout << "error!!" << std::endl;
        return -1;
    }

    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* shaderSource = (char*)calloc(file_size, 1);

    file.read(shaderSource, file_size);
    std::vector<GLchar> log;

    unsigned int shader;
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    GLint status;
        
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        if (type == GL_VERTEX_SHADER)
            std::cout << "vert ";
        else if(type == GL_FRAGMENT_SHADER)
            std::cout << "frag ";
        getError(log, shader);
    }


    return shader;
}


 void LightSource::updatePos() {

     pos = - glm::normalize(dir) * distance;


 }

 void LightSource::SetDir() {
     
     glm::vec4 _dir = glm::vec4(1, 0, 0, 1);
     glm::mat4 rot_mat = glm::mat4(1);

     rot_mat = glm::rotate(rot_mat, glm::radians(rot.x), glm::vec3(1, 0, 0));
     rot_mat = glm::rotate(rot_mat, glm::radians(rot.y), glm::vec3(0, 1, 0));
     rot_mat = glm::rotate(rot_mat, glm::radians(rot.z), glm::vec3(0, 0, 1));

     _dir = rot_mat * _dir;

     dir = glm::vec3(_dir.x, _dir.y, _dir.z);
 }

 void LightSource::SetDir(glm::vec3 vec) {
     dir = glm::normalize(vec);
 }


 void LightSource::Delete() {
     name.clear();
     vertexShader_source.clear();
     fragmentShader_source.clear();
     glDeleteBuffers(1, &depthMapFBO);
     glDeleteTextures(1, &depthMap);
     glDeleteProgram(ShaderProgram);
 }

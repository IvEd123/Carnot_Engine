
#include "cube.h"
#include "../render/render.h"


Cube::Cube(glm::vec3 _size){
    size = _size;
    pos = glm::vec3(0, 0, 0);
    rot = glm::vec3(0, 0, 0);


    material = Material();
    
}

Cube::Cube(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _size, GLuint* _texture){
    pos = _pos;
    rot = _rot;
    size = _size;
    material.bindTexture(*_texture);
}

Cube::~Cube() {
    vertices.clear();
    material.~Material();
}

Cube::Cube() {
    size = glm::vec3(1, 1, 1);
    pos = glm::vec3(0, 0, 0);
    rot = glm::vec3(0, 0, 0);


    material = *(new Material());
}



void Cube::CreateVerticesLegacy() {
    std::filesystem::path cubePath = ".\\Degug\\resources\\meshes\\cube.obj";
    setModel(cubePath);

    //material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index], (*norm_vec3)[array_index]);
    material.createVAO_VBO(vertices);
}



void Cube::setModel(std::filesystem::path path) {
    model_path = path;
    OBJLoader_v((std::filesystem::current_path() / path).string(), this);
}

void Cube::Draw(){
    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    
    material.setModel(glm::mat4(1.0f));

      // glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_3D, material.getTexture()); 


    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eyepos");
    glUniform3f(uniEye, Player::Get().GetPos().x, Player::Get().GetPos().y, Player::Get().GetPos().z);

    GLuint uniPos = glGetUniformLocation(material.getShaderProgram(), "pos");
    glUniform3f(uniPos, pos.x, pos.y, pos.z);

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize,size.x, size.y,  size.z);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, material.sun_rot->x, material.sun_rot->y, material.sun_rot->z);

    uniLight = glGetUniformLocation(material.getShaderProgram(), "lightDistance");
    glUniform1f(uniLight, material.sun_dist);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::Draw(glm::vec3 cameraPos){
    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    
    material.setModel(glm::mat4(1.0f));

      // glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_3D, material.getTexture()); 


    //GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eyepos");
    //glUniform3f(uniEye, _pl.GetPos().x, _pl.GetPos().y, _pl.GetPos().z);

    GLuint uniPos = glGetUniformLocation(material.getShaderProgram(), "pos");
    glUniform3f(uniPos, pos.x, pos.y, pos.z);

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize,size.x, size.y,  size.z);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, material.sun_rot->x, material.sun_rot->y, material.sun_rot->z);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::Delete(){
    deleteArrays();
    

    material.Delete();
}
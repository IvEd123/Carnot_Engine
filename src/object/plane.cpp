#include "plane.h"
#include "player.h"


Plane::Plane(){
    pos = glm::vec3(0, 0, 0);
    rot = glm::vec3(0, 0, 0);

    material = Material();
}

Plane::~Plane(){
    vertices.clear();
    material.~Material();
}

void Plane::Delete(){
    deleteArrays();
    material.Delete();
}

void Plane::Draw(){
    UpdateModelMatrix();
    material.updateUniforms();
    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());
    material.setModel(glm::mat4(1.0f));

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize, size.x, size.y, size.z);
    
    GLuint uniRepeat = glGetUniformLocation(material.getShaderProgram(), "repeat");
    glUniform1f(uniRepeat, texture_repeat);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, Player::Get().GetPos().x, Player::Get().GetPos().y, Player::Get().GetPos().z);
    
    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, -material.sun_rot->x, -material.sun_rot->y, -material.sun_rot->z);

    uniLight = glGetUniformLocation(material.getShaderProgram(), "lightDistance");
    glUniform1f(uniLight, material.sun_dist);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());


    glActiveTexture(GL_TEXTURE1);
    unsigned int sm = *material.shadowmap;
    glBindTexture(GL_TEXTURE_2D, sm);

    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "skybox"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, material.getEnvironmentMap());


    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Plane::CreateVertices(){

    (*vert_vec3)[array_index] = {
         glm::vec3(0.0, 0.0, 0.0),
         glm::vec3(1.0, 0.0, 0.0),
         glm::vec3(1.0, 0.0, 1.0),

         glm::vec3(1.0, 0.0, 1.0),
         glm::vec3(0.0, 0.0, 1.0),
         glm::vec3(0.0, 0.0, 0.0),
    };
    (*uv_vec2)[array_index] = {
         glm::vec2( 0.0, 0.0),
         glm::vec2(1.0, 0.0),
         glm::vec2(1.0, 1.0),

         glm::vec2(1.0, 1.0),
         glm::vec2(0.0, 1.0),
         glm::vec2(0.0, 0.0)
    };
    (*norm_vec3)[array_index] = {
         glm::vec3(0.0f, 1.0f,  0.0f),
         glm::vec3(0.0f, 1.0f,  0.0f),
         glm::vec3(0.0f, 1.0f,  0.0f),
 
         glm::vec3(0.0f, 1.0f,  0.0f),
         glm::vec3(0.0f, 1.0f,  0.0f),
         glm::vec3(0.0f, 1.0f,  0.0f),
    };

    material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index], (*norm_vec3)[array_index]);

}


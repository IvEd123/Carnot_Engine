#include "mesh.h"
#include "../render/render.h"

Mesh::Mesh(std::string& path){
    pos = glm::vec3(0, 0, 0);
    rot = glm::vec3(0, 0, 0);
    OBJLoader(path, this);

    size = glm::vec3( 1, 1, 1);

    material = Material();
}

void Mesh::Delete(){
    deleteArrays();
    material.Delete();
    model_path.clear();
}

void Mesh::CreateVertices() {
    material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index],(* norm_vec3)[array_index]);
}

void Mesh::setModel(std::filesystem::path path) {
    model_path = path;
    OBJLoader(path.string(), this);
    CreateVertices();
}

Mesh::~Mesh(){
}

Mesh::Mesh(){
    pos = glm::vec3(0, 0, 0);
    rot = glm::vec3(0, 0, 0);
    material = *(new Material());
}

void Mesh::Draw() {
    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());

    material.setModel(glm::mat4(1.0));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize, size.x, size.y, size.z);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, Player::Get().GetPos().x, Player::Get().GetPos().y, Player::Get().GetPos().z);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, -material.sun_rot->x, -material.sun_rot->y, -material.sun_rot->z);
    
    uniLight = glGetUniformLocation(material.getShaderProgram(), "lightDistance");
    glUniform1f(uniLight, material.sun_dist);
    
    

    glActiveTexture(GL_TEXTURE1);
    unsigned int sm = *material.shadowmap;
    glBindTexture(GL_TEXTURE_2D, sm);

    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "skybox"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, material.getEnvironmentMap());

    glDrawArrays(GL_TRIANGLES, 0,  (*vert_vec3)[array_index].size());

    

    glUseProgram(0);
    glBindVertexArray(0);
}

#include "screen.h"
#include <glm/ext/matrix_transform.hpp>


void Screen::Delete(){
    deleteArrays();
    material.Delete();
    glDeleteBuffers(1, &frameBuffer);
    glDeleteBuffers(1, &depth_stencil_buff);
    glDeleteTextures(1, &tex);
}

Screen::Screen(){
    material = Material();
    CreateVertices();
    material.createVAO_VBO(vertices);
}

Screen::~Screen() {
    vertices.clear();
    material.~Material();
}

GLuint* Screen::getDepthSteencilBuffer(){
    return &depth_stencil_buff;
}

GLuint* Screen::getColorBuffer(){
    return &tex;
}

void Screen::CreateVertices(){
    vertices =  {
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };

}

void Screen::Draw() {

    glDisable(GL_DEPTH_TEST);

    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());

    glm::mat4 mat = glm::translate(glm::mat4(1.0), *material.sun_rot);
        
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}
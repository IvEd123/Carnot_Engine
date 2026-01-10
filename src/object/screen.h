#pragma once

#include "geometricObject.h"

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
    void                                    setModel(std::filesystem::path path) {}
    void                                    Delete();
    Screen();
    ~Screen();
};

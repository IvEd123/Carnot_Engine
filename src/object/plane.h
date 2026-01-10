#pragma once

#include "geometricObject.h"


class Plane : public GeometricObject {
public:
    
    Plane();
    ~Plane();
    void                                    Delete();
    void                                    Draw();
    void                                    CreateVertices();
    void                                    setModel(std::filesystem::path path) {}
    int                                     texture_repeat = 1;

    GLuint *                                getNormalMap() { return &norm; }
    GLuint *                                getSpecMap() { return &spec; }
private:
    GLuint                                  norm;
    GLuint                                  spec;
};


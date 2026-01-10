#pragma once

#include "geometricObject.h"


class  Mesh : public GeometricObject {
public:
    std::filesystem::path                             model_path;
    Mesh(std::string& path);
    void                                    Delete();
    void                                    setModel(std::filesystem::path path);
    Mesh();
    ~Mesh();
    void                                    Draw();
    void                                    CreateVertices();
};

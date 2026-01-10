#include "geometricObject.h"


class Cube : public GeometricObject {
private:
    std::filesystem::path model_path;
    
public:
    Cube(glm::vec3 _size);
    Cube(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _size, GLuint* _texture);
    Cube();
    ~Cube();

    void                                    Draw();
    void                                    Draw(glm::vec3);
    void                                    Delete();
    void                                    CreateVertices() {};
    void                                    CreateVerticesLegacy();
    void                                    setModel(std::filesystem::path path);

    glm::vec3 size_v;

};
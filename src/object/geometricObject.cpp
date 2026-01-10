#include "geometricObject.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <gl/GLU.h>
#include <glm/ext/matrix_transform.hpp>
#include "../script/dllScriptHandler.h"
#include "light_source.h"

void GeometricObject::UpdateModelMatrix() {
    material.setModel(glm::translate(material.getModel(), pos));
    material.setModel(glm::rotate(material.getModel(), glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0)));
    material.setModel(glm::rotate(material.getModel(), glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0)));
    material.setModel(glm::rotate(material.getModel(), glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0)));
}

void GeometricObject::addLightSource(LightSource* source) {
    material.lightSpaceMatrixPtr = source->getProjMatrix();
    material.shadowmap = source->getShadowMap();
    material.sun_rot = (glm::vec3*)source->GetDirPtr();
    material.sun_dist = source->GetDistance();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "shadowMap"), 1);
    glUseProgram(0);
    glBindVertexArray(0);
}



void GeometricObject::decrementIndex() {
    array_index--;
}

void GeometricObject::deleteArrays() {
    vertices.clear();

    std::vector<std::vector<glm::vec3>>::iterator _vert_iter = vert_vec3->begin() + array_index;
    std::vector<std::vector<glm::vec3>>::iterator _norm_iter = norm_vec3->begin() + array_index;
    std::vector<std::vector<glm::vec2>>::iterator _uv_iter = uv_vec2->begin() + array_index;

    vert_vec3->erase(_vert_iter);
    norm_vec3->erase(_norm_iter);
    uv_vec2->erase(_uv_iter);

}

void GeometricObject::SetName(std::string _name) {
    name = _name;
}

std::string GeometricObject::GetName(void) {
    return name;
}

std::string* GeometricObject::GetNamePtr(void) {
    return &name;
}

glm::vec3 GeometricObject::GetPos() {
    return pos;
}

glm::vec3* GeometricObject::GetPosPtr() {
    return &pos;
}

void GeometricObject::SetPos(glm::vec3 _pos) {
    pos = _pos;
}

glm::vec3 GeometricObject::GetRot() {
    return rot;
}

glm::vec3* GeometricObject::GetRotPtr() {
    return &rot;
}

void GeometricObject::SetRot(glm::vec3 _rot) {
    rot = _rot;
}

void GeometricObject::SetSize(glm::vec3 s) {
    size = s;
}

glm::vec3 GeometricObject::GetSize() {
    return size;
}

glm::vec3* GeometricObject::GetSizePtr() {
    return &size;
}

void GeometricObject::SetType(int t) {
    type = t;
}
int GeometricObject::GetType() {
    return type;
}

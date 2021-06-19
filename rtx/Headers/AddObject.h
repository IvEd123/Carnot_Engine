#pragma once
#ifndef ADD_OBJECT_H
#define ADD_OBJECT_H

#include <vector>
#include "../Headers/GeometricObject.h"

#define LAST_OBJ_PTR obj_list.back()

extern std::vector<std::vector<glm::vec3>> array_of_vertecies;
extern std::vector<std::vector<glm::vec3>> array_of_normals;
extern std::vector<std::vector<glm::vec2>> array_of_uvs;

extern std::vector <GeometricObject*> obj_list;
extern std::vector <LightSource*> light_list;



void set_pointers_to_arrays(GeometricObject* g);
void AddObject(GeometryType type, std::string name);
GeometricObject* getByName(std::string name);


#endif
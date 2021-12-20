#include "../Headers/AddObject.h"


void set_pointers_to_arrays(GeometricObject* object) {
    //push vector3f to each array
    array_of_vertecies.emplace_back();
    array_of_normals.emplace_back();
    array_of_uvs.emplace_back();

    //copy pointers to arrays with data intp object
    object->vert_vec3 = &array_of_vertecies;
    object->norm_vec3 = &array_of_normals;
    object->uv_vec2 = &array_of_uvs;
    object->array_index = array_of_vertecies.size() - 1;
}

void AddObject(GeometryType type, std::string name) {

    //creating pointer to object
    GeometricObject* object;

    switch (type) {
    case CUBE:
        object = new Cube();

        break;
    case TERRAIN:
        object = new Terrain();

        break;
    case MESH:
        object = new Mesh();

        break;
    case PLANE:
        object = new Plane();

        break;
    default:
        throw std::exception("Unknown geometric object type");
        break;
    }

    //pushing pointer to object to list of objects
    obj_list.push_back(object);
    obj_list.back()->SetName(name);
    obj_list.back()->SetType(type);
    obj_list.back()->SetSize(sf::Vector3f(1, 1, 1));

    //set pointers to arrays with vertices, uvs and normals to object
    set_pointers_to_arrays(obj_list.back());

    //generate vertices is object isn't mesh which loads them from .obj
    if (type != MESH)
        obj_list.back()->CreateVertices();
}

GeometricObject* getByName(std::string name) {
    //finds object by name. slow
    for (int i = 0; i < obj_list.size(); i++)
        if (obj_list[i]->GetName() == name)
            return obj_list[i];
    std::cout << "object " << name << " not found" << std::endl;
    return nullptr;
}

void DeleteObject(int index) {
    obj_list[index]->~GeometricObject();
    std::vector<GeometricObject*>::iterator _iter = obj_list.begin() + index;
    obj_list.erase(_iter);
    for (int i = index; i < obj_list.size(); i++)
        obj_list[i]->DecrementIndex();
}
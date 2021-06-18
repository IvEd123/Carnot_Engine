#include "../Headers/AddObject.h"

void set_pointers_to_arrays(GeometricObject* g) {
    array_of_vertecies.emplace_back();
    array_of_normals.emplace_back();
    array_of_uvs.emplace_back();
    g->vert_vec3 = &array_of_vertecies;
    g->norm_vec3 = &array_of_normals;
    g->uv_vec2 = &array_of_uvs;
    g->array_index = array_of_vertecies.size() - 1;
}

void AddObject(GeometryType type, std::string name) {
    GeometricObject* object;
    std::vector<GeometricObject*>* vect = &obj_list;
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
        break;
    }
    vect->push_back(object);
    vect->back()->SetName(name);
    vect->back()->SetType(type);
    vect->back()->SetSize(1);
    set_pointers_to_arrays(vect->back());
    if (type != MESH)
        vect->back()->CreateVertices();
}

GeometricObject* getByName(std::string name) {
    for (int i = 0; i < obj_list.size(); i++)
        if (obj_list[i]->GetName() == name)
            return obj_list[i];
    std::cout << "object " << name << " not found" << std::endl;
    return nullptr;

}
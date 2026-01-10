#ifndef GUI_H
#define GUI_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <imgui.h>
#include <string>

#include "../object/geometricObject.h"
#include "../object/cloud_box.h"

class GUI_Element;

class GUI_Element {
private:

public:

};

class GUI_Manager {
private:

public:

};

class GUI_Vector3 {
public:
	glm::vec3 vec;
	void Show();
};

/* Object window
* Vector3 Position
* Vector3 rotation
* Float size
* Model path
* 
* Material
* * Textures paths
* * Vertex shader source
* * Fragment shader source
* * params of shader
* 
* Apply changes
*/

float* vec_to_ptr(glm::vec3);

class GUI_Object {
public:
	GUI_Object(GeometricObject*);
	GUI_Object();
	void Update();
	void SetObject(GeometricObject*);
protected:
	char default_name[50] = "No object selected";
	glm::vec3 default_vec = glm::vec3(0, 0, 0);
	float default_float = 0;
	char *name;
	glm::vec3 *pos;
	glm::vec3 *rot;
	glm::vec3* size;
};

class GUI_cloud : public GUI_Object  {
public:
	void Update();
	void SetObject(GeometricObject*);
private:
	struct Cloudbox::CloudParams* cloudParamsStruct;
};

#endif // !GUI_H
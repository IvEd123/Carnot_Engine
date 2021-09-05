#ifndef GUI_H
#define GUI_H

#include <SFML\System\Vector2.hpp>
#include <SFML\System\Vector3.hpp>
#include "imgui.h"
#include <string>

#include "../Headers/GeometricObject.h"

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
	sf::Vector3f vec;
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

float* vec_to_ptr(sf::Vector3f);

class GUI_Object {
public:
	GUI_Object(GeometricObject*);
	GUI_Object();
	void Update();
	void SetObject(GeometricObject*);
protected:
	char default_name[50] = "No object selected";
	sf::Vector3f default_vec = sf::Vector3f(0, 0, 0);
	float default_float = 0;
	char *name;
	sf::Vector3f *pos;
	sf::Vector3f *rot;
	sf::Vector3f* size;
};

class GUI_cloud : public GUI_Object  {
public:
	void Update();
	void SetObject(GeometricObject*);
private:
	struct Cloudbox::CloudParams* cloudParamsStruct;
};

#endif // !GUI_H
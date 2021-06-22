#include "../Headers/gui.h"

using namespace ImGui;



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

float* vec_to_ptr(sf::Vector3f *vec) {
	return (float*)vec;
}

GUI_Object::GUI_Object(GeometricObject * obj){
	SetObject(obj);
}

GUI_Object::GUI_Object(){
	name = default_name;
	pos = &default_vec;
	rot = &default_vec;
	size = &default_float;
}

void GUI_Object::SetObject(GeometricObject* obj) {
	name = (char*)&((obj->GetNamePtr())[0]) + 4;
	pos = obj->GetPosPtr();
	rot = obj->GetRotPtr();
	size = obj->GetSizePtr();
}

void GUI_Object::Update() {
	Begin(name);
	InputFloat3("Position", vec_to_ptr(pos));
	InputFloat3("Rotation", vec_to_ptr(rot));
	InputFloat("Size", size);
	if (TreeNode("material"))
		Text("mat");


	End();
}
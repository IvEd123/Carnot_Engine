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
	size = &default_vec;
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
	InputFloat3("Size", vec_to_ptr(size));
	if (TreeNode("material"))
		Text("mat");


	End();
}

void GUI_cloud::Update(){
    ImGui::Begin("cloud");
    InputFloat3("Position", vec_to_ptr(pos));
    InputFloat3("Rotation", vec_to_ptr(rot));
    InputFloat3("Size",  vec_to_ptr(size));
    ImGui::Text("March settings");
    ImGui::InputInt("Steps for shape", &(cloudParamsStruct->m_num_of_steps_shape));
    ImGui::InputInt("Steps for shading", &(cloudParamsStruct->m_num_of_steps_light));
    ImGui::Text("Base shape");
    ImGui::InputFloat3("Cloud scale", &(cloudParamsStruct->m_cloud_scale.x));
    ImGui::InputFloat3("Cloud offset", &(cloudParamsStruct->m_cloud_offset.x));
    ImGui::SliderFloat("Threshold", &(cloudParamsStruct->m_density_threshold), 0.0, 1.0);
    ImGui::SliderFloat("Density Multiplier", &(cloudParamsStruct->m_density_multiplier), 0.0, 100.0);
    ImGui::Text("Detail parametrs");
    ImGui::InputFloat3("Noise 2nd layer scale", &(cloudParamsStruct->m_second_layer_scale.x));
    ImGui::InputFloat3("Noise 2nd layer offset", &(cloudParamsStruct->m_second_layer_offset.x));
    ImGui::InputFloat3("Noise 3rd layer scale", &(cloudParamsStruct->m_third_layer_scale.x));
    ImGui::InputFloat3("Noise 3rd layer offset", &(cloudParamsStruct->m_third_layer_offset.x));
    ImGui::Text("Lighting");
    ImGui::ColorPicker3("Light Color", &(cloudParamsStruct->m_light_color.x));
    ImGui::SliderFloat("light Absorption Through Cloud", &(cloudParamsStruct->m_light_absorption_through_cloud), 0.0, 1.0);
    ImGui::SliderFloat("light Absorption Toward Sun", &(cloudParamsStruct->m_light_absorption_toward_light), 0.0, 2.0);
    ImGui::SliderFloat("darknessThreshold", &(cloudParamsStruct->m_darkness_threshold), 0.0, 1.0);

    ImGui::SliderFloat("Froward scattering", &(cloudParamsStruct->m_phase_params.x), 0.0, 1.0);
    ImGui::SliderFloat("Back scattering", &(cloudParamsStruct->m_phase_params.y), 0.0, 1.0);
    ImGui::SliderFloat("Base brightness", &(cloudParamsStruct->m_phase_params.z), 0.0, 1.0);
    ImGui::SliderFloat("Phase factor", &(cloudParamsStruct->m_phase_params.w), 0.0, 1.0);

    ImGui::End();
}

void GUI_cloud::SetObject(GeometricObject* obj){
    name = (char*)&((obj->GetNamePtr())[0]) + 4;
    pos = obj->GetPosPtr();
    rot = obj->GetRotPtr();
    size = obj->GetSizePtr();
    cloudParamsStruct = &((Cloudbox*)obj)->cloudParams;
}

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

void GUI_cloud::Update(){
    ImGui::Begin("cloud");
    InputFloat3("Position", vec_to_ptr(pos));
    InputFloat3("Rotation", vec_to_ptr(rot));
    InputFloat("Size", size);
    ImGui::Text("March settings");
    ImGui::InputInt("Steps for shape", &(cloudParamsStruct->num_of_steps));
    ImGui::InputInt("Steps for shading", &(cloudParamsStruct->num_of_steps_inside));
    ImGui::Text("Base shape");
    ImGui::InputFloat3("Cloud scale", &(cloudParamsStruct->cloudScale.x));
    ImGui::InputFloat3("Cloud offset", &(cloudParamsStruct->cloudOffset.x));
    ImGui::SliderFloat("Threshold", &(cloudParamsStruct->DensityThreshold), 0.0, 1.0);
    ImGui::SliderFloat("Density Multiplier", &(cloudParamsStruct->DensityMultiplier), 0.0, 100.0);
    ImGui::Text("Detail parametrs");
    ImGui::InputFloat3("Noise 2nd layer scale", &(cloudParamsStruct->secondLayerScale.x));
    ImGui::InputFloat3("Noise 2nd layer offset", &(cloudParamsStruct->secondLayerOffset.x));
    ImGui::InputFloat3("Noise 3rd layer scale", &(cloudParamsStruct->thirdLayerScale.x));
    ImGui::InputFloat3("Noise 3rd layer offset", &(cloudParamsStruct->thirdLayerOffset.x));
    ImGui::Text("Lighting");
    ImGui::ColorPicker3("Light Color", &(cloudParamsStruct->LightColor.x));
    ImGui::SliderFloat("light Absorption Through Cloud", &(cloudParamsStruct->lightAbsorptionThroughCloud), 0.0, 1.0);
    ImGui::SliderFloat("light Absorption Toward Sun", &(cloudParamsStruct->lightAbsorptionTowardSun), 0.0, 2.0);
    ImGui::SliderFloat("darknessThreshold", &(cloudParamsStruct->darknessThreshold), 0.0, 1.0);

    ImGui::SliderFloat("Froward scattering", &(cloudParamsStruct->phaseParams.x), 0.0, 1.0);
    ImGui::SliderFloat("Back scattering", &(cloudParamsStruct->phaseParams.y), 0.0, 1.0);
    ImGui::SliderFloat("Base brightness", &(cloudParamsStruct->phaseParams.z), 0.0, 1.0);
    ImGui::SliderFloat("Phase factor", &(cloudParamsStruct->phaseParams.w), 0.0, 1.0);

    ImGui::End();
}

void GUI_cloud::SetObject(GeometricObject* obj){
    name = (char*)&((obj->GetNamePtr())[0]) + 4;
    pos = obj->GetPosPtr();
    rot = obj->GetRotPtr();
    size = obj->GetSizePtr();
    cloudParamsStruct = &((Cloudbox*)obj)->cloudParams;
}

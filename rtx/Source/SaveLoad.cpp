#include "../Headers/SaveLoad.h"

//using namespace std;

void SaveLoad::SetPath(std::string _path){
	path = _path;
}

std::string SaveLoad::GetPath(){
	return path;
}

void SaveLoad::SetName(std::string _name){
	name = _name;
}

std::string SaveLoad::GetName(){
	return name;
}

void SaveLoad::SetExtension(std::string _ex){
	extension = _ex;
}

std::string SaveLoad::GetExtension(){
	return extension;
}

int SaveLoad::Load(std::vector<GeometricObject*>* obj_list, std::vector<LightSource*>* light_list){
	std::string  path_full = path + "/" + name + "." + extension;
	std::ifstream file;
	file.open(path_full);

	if (!file.is_open()) {
		std::cout << "not opened" << std::endl;
		return 1;
	}
	char buffer[50];

	int obj_nun;

	file >> buffer;
	if (buffer == "#objects")
		file >> obj_nun;
	else {
		std::cout << "unknown command" << std::endl;
		return 1;
	}

	int type;

	while (!file.eof()) {
		file >> buffer;
		if (buffer[0] != '#') {
			std::cout << "unknown command" << std::endl;
			return 1;
		}
		else if (buffer == "#t") {		// type
			file >> type;
		}
		else if (buffer == "#n") {		// name

		}
		else if (buffer == "#m") {		// mesh

		}
		else if (buffer == "#s") {		// size

		}
		else if (buffer == "#p") {		// position

		}
		else if (buffer == "#r") {		// rotation

		}
		else if (buffer == "#vs") {		// vertex shader

		}
		else if (buffer == "#fs") {		// fragment shader

		}
		else if (buffer == "#sh") {		// shadows

		}
		else if (buffer == "#end") {	// end

		}
	}

	return 0;
}

int SaveLoad::Save(std::vector<GeometricObject*>* obj_list, std::vector<LightSource*>* light_list){

	std::string  path_full = path + "/" + name + "." + extension;
	std::ofstream file;
	file.open(path_full);

	if (!file.is_open()) {
		std::cout << "not opened" << std::endl;
		return 1;
	}

	//objects

	file << "#objects " << obj_list->size() << std::endl << std::endl;
	
	for (int i = 0; i < obj_list->size(); i++) {
		file << "#t " << (*obj_list)[i]->GetType() << std::endl;
		file << "#n " << (*obj_list)[i]->GetName() << std::endl;
		if ((*obj_list)[i]->GetType() == MESH)
			file << "#m " << ((Mesh*)(*obj_list)[i])->model_path << std::endl;
		file << "#s " << (*obj_list)[i]->GetSize() << std::endl;
		file << "#p " << (*obj_list)[i]->GetPos().x << " " << (*obj_list)[i]->GetPos().y << " " << (*obj_list)[i]->GetPos().z << " " << std::endl;
		file << "#r " << (*obj_list)[i]->GetRot().x << " " << (*obj_list)[i]->GetRot().y << " " << (*obj_list)[i]->GetRot().z << " " << std::endl;
		file << "#vs " << (*obj_list)[i]->material.GetVSPath() << std::endl;
		file << "#fs " << (*obj_list)[i]->material.GetFSPath() << std::endl;
		file << "#sh " << (*obj_list)[i]->cast_shadow << std::endl;
		file << "#end" << std::endl;
		file << std::endl;
	}
	file.close();
	return 0;
}

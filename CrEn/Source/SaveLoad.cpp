#include "../Headers/SaveLoad.h"

void SaveLoad::SetPath(std::string& _path){
	path = _path;
}

std::string SaveLoad::GetPath(){
	return path;
}

void SaveLoad::SetName(std::string& _name){
	name = _name;
}

std::string SaveLoad::GetName(){
	return name;
}

void SaveLoad::SetExtension(std::string& _ex){
	extension = _ex;
}

std::string SaveLoad::GetExtension(){
	return extension;
}

int SaveLoad::Load(){
	std::string  path_full = path + "/" + name + "." + extension;
	std::ifstream file;
	file.open(path_full);

	if (!file.is_open()) {
		std::cout << "not opened" << std::endl;
		return 1;
	}
	std::string buffer;

	int obj_num;

	file >> buffer;
	if (buffer.compare("#objects") == 0)
		file >> obj_num;
	else {
		std::cout << "unknown command" << std::endl;
		return 1;
	}

	obj_list.reserve(sizeof(GeometricObject*) * obj_num);

	int type;
	file >> buffer;
	while (!file.eof() && buffer.compare("#scripts") != 0) {
		bool loaded = false;
		
		if (buffer.compare("#t") == 0){
			file >> type;
			std::string name;
			file >> buffer;
			if (buffer.compare("#n") == 0) {
				file >> name;
				AddObject((GeometryType)type, name);
				file >> buffer;
				if (buffer.compare("#m") == 0 && type == MESH) {
					std::string mesh_path;
					//char mesh_path[100];
					file >> mesh_path;
					LAST_OBJ_PTR->SetModel(mesh_path);
					file >> buffer;
				}
				sf::Vector3f size;
				
				if (buffer.compare("#s") == 0) {
					file >> size.x;
					file >> size.y;
					file >> size.z;
					LAST_OBJ_PTR->SetSize(size);

					file >> buffer;
					if (buffer.compare("#p") == 0) {
						sf::Vector3f pos;
						file >> pos.x;
						file >> pos.y;
						file >> pos.z;
						LAST_OBJ_PTR->SetPos(pos);

						file >> buffer;
						if (buffer.compare("#r") == 0) {
							sf::Vector3f rot;
							file >> rot.x;
							file >> rot.y;
							file >> rot.z;
							LAST_OBJ_PTR->SetRot(rot);

							file >> buffer;
							if (buffer.compare("#vs") == 0) {
								char shader_path[100];
								file >> shader_path;
								LAST_OBJ_PTR->material.loadShader(GL_VERTEX_SHADER, shader_path);
								file >> buffer;
								if (buffer.compare("#fs") == 0) {
									file >> shader_path;
									LAST_OBJ_PTR->material.loadShader(GL_FRAGMENT_SHADER, shader_path);
									int error = LAST_OBJ_PTR->material.CreateShaders();
									LAST_OBJ_PTR->material.specifyVertexAttributes_mesh();
									if (error != 0) {
										std::cout << name << " script error " << error << std::endl;
									}
									file >> buffer;
									if (buffer.compare("#sh") == 0) {
										bool sh;
										file >> sh;
										LAST_OBJ_PTR->cast_shadow = sh;
										//LAST_OBJ_PTR->addLightSource(light_list[0]);
										
										file >> buffer;
										if (buffer.compare("#tex") == 0) {
											char tex_path[100];
											file >> tex_path;
											LAST_OBJ_PTR->material.bindTexture(tex_path);
											int tex_repeat;
											file >> tex_repeat;
											LAST_OBJ_PTR->m_texture_repeat = tex_repeat;
											file >> buffer;
										}

										if (buffer.compare("#end") == 0)
											loaded = true;
									}
								}
							}
						}
					}
				}
			}
		}
		if (!loaded)
			std::cout << "error " << name << std::endl;
		file >> buffer;
	}



	if (buffer.compare("#scripts") == 0) {
		int scr_n;
		file >> scr_n;
		if (scr_n > 0) {
			while (!file.eof()) {
				file >> buffer;
				if (buffer.compare("#id") == 0) {
					int id;
					file >> id;
					file >> buffer;
					if (buffer.compare("#path") == 0) {
						char path[100];
						file >> path;
						scripts.push_back(DLLScriptHandler());
						scripts.back().setObj(obj_list[id], id);
						scripts.back().SetDLL(path);
					}
				}

			}
		}
	}

	return 0;
}

typedef struct obj_temp {
	int type, shadow_cast;
	std::string name, vs_path, fs_path, texture_path, model_path;
	float texture_repeat;
	sf::Vector3f pos, rot, scale;
};

void parse_string(const std::string& tag, std::vector<std::string> values, obj_temp &obj) {
	if (tag.compare("#t") == 0) {
		obj.type = std::stoi(values[0]);
	}
	else if (tag.compare("#n") == 0) {
		obj.name = values[0];
	}
	else if (tag.compare("#m") == 0) {
		obj.model_path = values[0];
	}
	else if (tag.compare("#s") == 0) {
		obj.scale = {std::stof(values[0]), std::stof(values[1]), std::stof(values[2])};
	}
	else if (tag.compare("#p") == 0) {
		obj.pos = {std::stof(values[0]), std::stof(values[1]), std::stof(values[2])};
	}
	else if (tag.compare("#r") == 0) {
		obj.rot = {std::stof(values[0]), std::stof(values[1]), std::stof(values[2])};
	}
	else if (tag.compare("#vs") == 0) {
		obj.vs_path = values[0];
	}
	else if (tag.compare("#fs") == 0) {
		obj.fs_path = values[0];
	}
	else if (tag.compare("#tex") == 0) {
		obj.texture_path = values[0];
		obj.texture_repeat = stof(values[1]);
	}
	else if (tag.compare("#sh") == 0) {
		obj.shadow_cast =  stoi(values[0]);
	}
}

int SaveLoad::Load(Item& root) {
	std::string  path_full = path + "/" + name + "." + extension;
	std::ifstream file;
	file.open(path_full);

	if (!file.is_open()) {
		std::cout << "not opened" << std::endl;
		return 1;
	}
	std::string buffer;

	int obj_num;
	file >> buffer;

	if (buffer.compare("#objects") == 0)
		file >> obj_num;
	else {
		std::cout << "unknown command" << std::endl;
		return 1;
	}
	

	for (int i = 0; i < obj_num; i++) {
		obj_temp obj_properties;
		file >> buffer;
		std::string tag;
		std::vector<std::string> values;
		while (buffer.compare("#end") != 0) {
			values.clear();
			tag = buffer;
			file >> buffer;
			while (buffer[0] != '#') {
				values.push_back(buffer);
				file >> buffer;
			}
				
			parse_string(tag, values, obj_properties);
		}
		AddObject((GeometryType)obj_properties.type, obj_properties.name);
		LAST_OBJ_PTR->SetPos(obj_properties.pos);
		LAST_OBJ_PTR->SetRot(obj_properties.rot);
		LAST_OBJ_PTR->SetSize(obj_properties.scale);
		LAST_OBJ_PTR->cast_shadow = obj_properties.shadow_cast;
		LAST_OBJ_PTR->material.loadShader(GL_VERTEX_SHADER, obj_properties.vs_path.c_str());
		LAST_OBJ_PTR->material.loadShader(GL_FRAGMENT_SHADER, obj_properties.fs_path.c_str());
		LAST_OBJ_PTR->material.bindTexture(obj_properties.texture_path.c_str());
		LAST_OBJ_PTR->m_texture_repeat = obj_properties.texture_repeat;
		//loadModel(obj_properties.model_path, LAST_OBJ_PTR, &root);

		Item* node = root.AddChild(LAST_OBJ_PTR);
		if(obj_properties.type == MESH)
			loadModel(obj_properties.model_path, LAST_OBJ_PTR, node);

	}


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
		file << "#s " << (*obj_list)[i]->GetSize().x << (*obj_list)[i]->GetSize().y << (*obj_list)[i]->GetSize().z << std::endl;
		file << "#p " << (*obj_list)[i]->GetPos().x << " " << (*obj_list)[i]->GetPos().y << " " << (*obj_list)[i]->GetPos().z << " " << std::endl;
		file << "#r " << (*obj_list)[i]->GetRot().x << " " << (*obj_list)[i]->GetRot().y << " " << (*obj_list)[i]->GetRot().z << " " << std::endl;
		file << "#vs " << (*obj_list)[i]->material.GetVSPath() << std::endl;
		file << "#fs " << (*obj_list)[i]->material.GetFSPath() << std::endl;
		file << "#sh " << (*obj_list)[i]->cast_shadow << std::endl;
		file << "#tex " << (*obj_list)[i]->material.tex_path << " " << (*obj_list)[i]->m_texture_repeat << std::endl;
		file << "#end" << std::endl;
		file << std::endl;
	}

	file << "#scripts " << scripts.size() << std::endl << std::endl;
	for (int i = 0; i < scripts.size(); i++) {
		file << "#id " << scripts[i].obj_id << std::endl;
		file << "#path " << scripts[i].path << std::endl;
	}

	file.close();
	return 0;
}

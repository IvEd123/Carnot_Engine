#include "../Headers/ScriptCompiler.h"
#include "../Headers/GeometricObject.h"

ScriptHandler ScriptCompiler::LoadScript(std::string path, GeometricObject* obj) {


	ScriptHandler sh = ScriptHandler();

	sh.object = obj;

	std::ifstream file;
	file.open(path);
	if (!file.is_open()) {
		std::cout << "could not open script file" << std::endl;
		return sh;
	}

	sh.path = path;

	std::string s;
	std::vector<uint8_t>* dest = 0;

	bool error = false;
	std::string error_log;
	sh.bc = 0;

	int arguments_left = 0;
	int prev_inst = 0;
	bool name = false;
	bool equal = false;

	while (!file.eof() && !error) {
		file >> s;
	
		uint8_t kw = keywords[s];

		if (arguments_left == 0) {
			switch (kw) {
			case START_F:
				if (sh.bc == 0)
					dest = &(sh.start_inst);
				else {
					error = true;
					error_log = "using keyword 'start' in function\n";
				}
				break;
			case UPDATE_F:
				if (sh.bc == 0)
					dest = &(sh.update_inst);
				else {
					error = true;
					error_log = "using keyword 'update' in function\n";
				}
				break;
			case BRACKET_L:
				sh.bc++;
				break;
			case BRACKET_R:
				sh.bc--;
				if (sh.bc < 0) {
					error = true;
					error_log = "unexpected '}'\n";
				}
				break;
			case INT_VAR:
			case FLOAT_VAR:
				name = true;
			case ROTATE:
			case ROTATETO:
			case MOVE:
			case MOVETO:
				prev_inst = kw;
				arguments_left = arguments[kw];
				dest->push_back (kw);
				break;
			default:
				error = true;
				error_log = "unknown command " + s + "\n";
				break;
			}
		}
		else {
			if (s == "=")
					continue;
			if (!name) {
				switch (prev_inst) {
				case INT_VAR:
					sh.int_variables.push_back(std::stoi(s));
					break;
				case FLOAT_VAR:
					sh.float_variables.push_back(std::stof(s));
					break;
				case MOVE:
				case MOVETO:
				case ROTATE:
				case ROTATETO:
					sh.float_variables.push_back(std::stof(s));
					break;
				default:
					break;
				}
				arguments_left--;
			}
			else {
				
				switch (prev_inst) {
				case INT_VAR:
					sh.int_var_names.insert(std::pair<std::string, int>(s, sh.int_variables.size()));
					equal = true;
					break;
				case FLOAT_VAR:
					sh.float_var_names.insert(std::pair<std::string, int>(s, sh.float_variables.size()));
					equal = true;
					break;
				default:
					break;
				}
				name = false;
				
			}
		}
		
	}
	if (error)
		std::cout << error_log;


	return sh;
}

void ScriptHandler::Update(){
	for (int i = 0; i < update_inst.size(); i++) {
		uint8_t inst = update_inst[i];
		int arg_num = arguments[inst];

		

		switch (inst){
		case MOVE:

		default:
			break;
		}
	}
}

void ScriptHandler::Start(){

}

void ScriptHandler::move(sf::Vector3f v){
	object->SetPos(object->GetPos() + v);
}

void ScriptHandler::moveto(sf::Vector3f v){
	object->SetPos(v);
}

void ScriptHandler::rotate(sf::Vector3f v){
	object->SetRot(object->GetRot() + v);
}

void ScriptHandler::rotateto(sf::Vector3f v){
	object->SetRot(v);
}

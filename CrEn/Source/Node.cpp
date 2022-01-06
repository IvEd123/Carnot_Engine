#include "../Headers/Node.h"

Item::Item(){
	isRoot = true;
	obj = nullptr;
	id = 0;
	level = 0;
	parent = nullptr;
}

Item::Item(GeometricObject* _obj){
	isRoot = true;
	level = 0;
	id = 0;
	obj = _obj;
	parent = nullptr;
}


Item::Item(Item* _parent, GeometricObject* _obj, int _id){
	parent = _parent;
	isRoot = parent == nullptr;
	obj = _obj;
	id = _id;
	level = parent->GetLevel()+1;
}

Item::~Item() {
	for (int i = 0; i < children.size(); i++)
		children[i]->~Item();
	children.clear();
	std::cout << "Item tree node " << obj->GetName() <<  " deleted\n";
}

Item* Item::AddChild(GeometricObject* _obj){
	if (_obj == nullptr)
		throw std::exception("pointer to object is nullptr");

	int child_id = children.size();
	Item *child = new Item(this, _obj, child_id);
	children.push_back(child);
	return child;
}

const GeometricObject* Item::GetObject(){
	return obj;
}

void Item::Draw(){
	if(obj != nullptr)
		obj->Draw();
	for (int i = 0; i < children.size(); i++)
		children[i]->Draw();
	
}

bool Item::DrawIf(bool(*func)(Item* _this_ptr)){
	if (obj != nullptr)
		throw std::exception("Unable to draw non-existent geometric object");

	if (func(this)) {
		obj->Draw();
		for (int i = 0; i < children.size(); i++)
			children[i]->DrawIf(func);
		return true;
	}
	return false;
}

void Item::DrawExternalShader(int shader_program, const GLfloat* matrix_prt){
	if (obj != nullptr) {
		sf::Vector3f pos = obj->GetPos();
		sf::Vector3f rot = obj->GetRot();

		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, ConvertSFML2GLM(pos));
            
		model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));

		GLuint lightSpaceMatrixLocation = glGetUniformLocation(shader_program, "mvpMatrix");
		glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, matrix_prt);

		GLuint model_loc = glGetUniformLocation(shader_program, "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint size_loc = glGetUniformLocation(shader_program, "size");
		glUniform3f(size_loc, obj->GetSize().x, obj->GetSize().y, obj->GetSize().z);

		glBindVertexArray(obj->material.getVAO());

		int size = (*(*obj).vert_vec3)[obj->array_index].size();
		glDrawArrays(GL_TRIANGLES, 0, size);
		glBindVertexArray(0);

	}
	for (int i = 0; i < children.size(); i++)
		children[i]->DrawExternalShader(shader_program, matrix_prt);
}

void Item::DrawExternalShaderIf(bool(*func)(Item* _this_ptr), int shader_program, const GLfloat* matrix_prt){
	if (obj != nullptr && func(this)) {
		sf::Vector3f pos = obj->GetPos();
		sf::Vector3f rot = obj->GetRot();

		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, ConvertSFML2GLM(pos));

		model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));

		GLuint lightSpaceMatrixLocation = glGetUniformLocation(shader_program, "mvpMatrix");
		glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, matrix_prt);

		GLuint model_loc = glGetUniformLocation(shader_program, "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint size_loc = glGetUniformLocation(shader_program, "size");
		glUniform3f(size_loc, obj->GetSize().x, obj->GetSize().y, obj->GetSize().z);

		glBindVertexArray(obj->material.getVAO());

		int size = (*(*obj).vert_vec3)[obj->array_index].size();
		glDrawArrays(GL_TRIANGLES, 0, size);
		glBindVertexArray(0);

	}
	for (int i = 0; i < children.size(); i++)
		children[i]->DrawExternalShaderIf(func, shader_program, matrix_prt);
}

int Item::GetLevel(){
	return level;
}

int Item::GetID(){
	return id;
}


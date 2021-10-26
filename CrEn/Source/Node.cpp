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

void Item::AddChild(GeometricObject* _obj){
	int child_id = children.size();
	children.emplace_back(this, _obj, child_id);
}

void Item::Draw(){
	if (obj != nullptr)
		obj->Draw();
}

bool Item::DrawIf(bool(*func)(Item* _this_ptr)){
	if (func(this)) {
		obj->Draw();
		return true;
	}
	return false;
}

int Item::GetLevel(){
	return level;
}

int Item::GetID(){
	return id;
}


#ifndef NODE_H
#define NODE_H

#include "../Headers/GeometricObject.h"

class Item {
public:
	Item();
	Item(GeometricObject* _obj);
	Item(Item* _parent, GeometricObject* _obj, int _id);
	~Item();

	Item* AddChild(GeometricObject* _obj);

	const GeometricObject* GetObject();

	void Draw();
	bool DrawIf(bool (*func)(Item* _this_ptr)); 
	void DrawExternalShader(int shader_program, const GLfloat* matrix_prt);
	void DrawExternalShaderIf(bool (*func)(Item* _this_ptr), int shader_program, const GLfloat* matrix_prt);

	int GetLevel();
	int GetID();

private:
	Item* parent;
	std::vector<Item*> children;
	GeometricObject* obj;
	bool isRoot;
	int level, id;
};


#endif // !NODE_H

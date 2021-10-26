#ifndef NODE_H
#define NODE_H

#include "../Headers/GeometricObject.h"

class Item {
public:
	Item();
	Item(GeometricObject* _obj);
	Item(Item* _parent, GeometricObject* _obj, int _id);
	~Item();

	void AddChild(GeometricObject* _obj);


	void Draw();
	bool DrawIf(bool (*func)(Item* _this_ptr)); 

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

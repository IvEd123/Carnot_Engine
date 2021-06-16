#pragma once

#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../Headers/GeometricObject.h"


class SaveLoad{
private:
	std::string path;
	std::string name;
	std::string extension = "sus";
public:
	void SetPath(std::string _path);
	std::string GetPath();
	
	void SetName(std::string _name);
	std::string GetName();
	
	void SetExtension(std::string _ex);
	std::string GetExtension();

	int Load(std::vector <GeometricObject*>* obj_list, std::vector <LightSource*>* light_list);
	int Save(std::vector <GeometricObject*>* obj_list, std::vector <LightSource*>* light_list);
};

#endif // !SAVELOAD_H
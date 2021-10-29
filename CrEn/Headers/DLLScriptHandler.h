#pragma once
#ifndef DLLSCRIPT_H_
#define DLLSCRIPT_H_

#define _CRT_SECURE_NO_WARNINGS


#include <SFML/System/Vector3.hpp>
#include "../Headers/GeometricObject.h"
#include <Windows.h>
#include <string>
#include <iostream>


//simplified gameobject struct that contains pointers to values that can be modified in scripts
typedef struct gameobject {
	sf::Vector3f* pos;
	sf::Vector3f* rot;
	sf::Vector3f* size;
};


typedef int (*UPDFUNCPTR) (gameobject*);
typedef int (*STARTFUNCPTR) (gameobject*);

class DLLScriptHandler{
	HINSTANCE hInst;
	gameobject obj;
	UPDFUNCPTR update;
	STARTFUNCPTR start;
	bool error = false;
public:
	void setObj(GeometricObject*, int id);

	int obj_id;

	void Update();
	void Start();

	int SetDLL(const char* path);

	std::string path;
};



#endif
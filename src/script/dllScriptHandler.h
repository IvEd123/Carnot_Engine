#pragma once
#ifndef DLLSCRIPT_H_
#define DLLSCRIPT_H_

#define _CRT_SECURE_NO_WARNINGS


#include <glm/vec3.hpp>
#include "../object/geometricObject.h"
#include <Windows.h>
#include <string>
#include <iostream>



typedef struct gameobject {
	glm::vec3* pos;
	glm::vec3* rot;
	glm::vec3* size;
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
#pragma once
#ifndef DLLSCRIPT_H_
#define DLLSCRIPT_H_

#define _CRT_SECURE_NO_WARNINGS


#include <SFML/System/Vector3.hpp>
#include <Windows.h>
#include <string>
#include <iostream>

#include "../Headers/GeometricObject.h"

typedef struct gameobject {
	sf::Vector3f* pos;
	sf::Vector3f* rot;
	float* size;
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
	void setObj(GeometricObject*);

	void Update();
	void Start();

	int SetDLL(const char* path);
};



#endif
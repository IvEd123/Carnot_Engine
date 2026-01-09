#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <corecrt_math_defines.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material;
class GeometricObject;

											//
class Player {
private:
	static glm::vec3						pos;
	static glm::vec3						velocity;
	static Player							s_Instance;
	static float							a;//acceleration
	static glm::vec2 						ang;
	float									h = 1.5;
	const float								max_speed = .25;
public:
	Player(float _h) { h = _h; }
	Player() {}

	glm::mat4								view;
	glm::mat4								proj;

	static Player&							Get() { return s_Instance; }
	
	static glm::vec2						GetAng() { return Get().ang; }
	
	static void								SetAng(glm::vec2 _ang) { Get().ang = _ang; }

	static glm::vec3						GetPos() { return Get().pos; }
	static void								SetPos(glm::vec3 _pos) { pos = _pos; }

	void									MoveLegacy(glm::vec2 ang);
	void									Move(glm::vec2 ang, float Dtime);
	void									SetAcceleration(float _a) { a = _a; }
	void									Update();
};

#endif
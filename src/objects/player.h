#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <corecrt_math_defines.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"

class Material;
class GeometricObject;

											//
class Player {
private:
	static sf::Vector3f						pos;
	static sf::Vector3f						velocity;
	static Player							s_Instance;
	static float							a;//acceleration
	static sf::Vector2f						ang;
	float									h = 1.5;
	const float								max_speed = .25;
public:
	Player(float _h) { h = _h; }
	Player() {}

	glm::mat4								view;
	glm::mat4								proj;

	static Player&							Get() { return s_Instance; }
	
	static sf::Vector2f						GetAng() { return Get().ang; }
	
	static void								SetAng(sf::Vector2f _ang) { Get().ang = _ang; }

	static sf::Vector3f						GetPos() { return Get().pos; }
	static void								SetPos(sf::Vector3f _pos) { pos = _pos; }

	void									MoveLegacy(sf::Vector2f ang);
	void									Move(sf::Vector2f ang, float Dtime);
	void									SetAcceleration(float _a) { a = _a; }
	void									Update();
};

#endif
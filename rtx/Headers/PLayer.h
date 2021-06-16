#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <corecrt_math_defines.h>

#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include "../thirdparty\glm\glm.hpp"
#include "../thirdparty\glm\gtc\matrix_transform.hpp"
#include "../thirdparty\glm\gtc\type_ptr.hpp"

class Material;
class GeometricObject;


class Player {
private:
	static sf::Vector3f pos;
	static Player s_Instance;
	static float speed;
	static sf::Vector2f ang;
	float h = 1.5;
public:
	glm::mat4 view;
	glm::mat4 proj;

	static Player& Get() {
		return s_Instance;
	}
	
	static sf::Vector2f GetAng() {
		return Get().ang;
	}
	
	static void SetAng(sf::Vector2f _ang) {    Get().ang = _ang;   }

	static sf::Vector3f GetPos() {  return Get().pos;  }
	static void SetPos(sf::Vector3f _pos) {   pos = _pos;  }

	void Move(sf::Vector2f ang);

	void SetSpeed(float _speed) { speed = _speed; }

	Player(float _h) {
		h = _h;
	}

	Player() {
	}


	void Update();
};

#endif
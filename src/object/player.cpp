#include "player.h"


void Player::Move(glm::vec2 ang, float Dtime) {
	
	return;

	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		pos.y += 0.1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		pos.y -= 0.1;

	if (velocity.x * velocity.x + velocity.z * velocity.z <= max_speed * max_speed) {

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			velocity.x += a * Dtime;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			velocity.x -= a * Dtime;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			velocity.z += a * Dtime;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			velocity.z -= a * Dtime;
	}
	float k = 4; // brake 

	if (!(
		(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::D)))) {
		if (velocity.x > 0)
			velocity.x = velocity.x - a * k < 0 ? 0 : velocity.x - a * k;
		else if (velocity.x < 0)
			velocity.x = velocity.x + a * k > 0 ? 0 : velocity.x + a * k;

		if (velocity.z > 0)
			velocity.z = velocity.z - a * k < 0 ? 0 : velocity.z - a * k;
		else if (velocity.z < 0)
			velocity.z = velocity.z + a * k > 0 ? 0 : velocity.z + a * k;
	}

	
	
	pos.z -= (velocity.x * cos(ang.x * 0.0174533) - velocity.z * sin(ang.x * 0.0174533) ) * Dtime;
	pos.x -= (velocity.x * sin(ang.x * 0.0174533) + velocity.z * cos(ang.x * 0.0174533) ) * Dtime;
	*/
}


void Player::Update(){
    
}

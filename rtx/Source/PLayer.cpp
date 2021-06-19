#include "../Headers/PLayer.h"

void Player::Move(sf::Vector2f ang) {

	//TODO: improve moving

    float dx = 0, dz = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		    dx = sin(ang.x/180*M_PI) * -speed; 
		    dz = cos(ang.x/180*M_PI) * -speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		dx = sin(ang.x/180*M_PI) * speed;
		dz = cos(ang.x/180*M_PI) * speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		dx = sin((ang.x+90)/180*M_PI) * speed;	
		dz = cos((ang.x+90)/180*M_PI) * speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))	
	{
		dx = sin((ang.x-90)/180*M_PI) * speed;	
		dz = cos((ang.x-90)/180*M_PI) * speed;
	}
    pos.x += dx;
    pos.z += dz;
}


void Player::Update(){
    
}

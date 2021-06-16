#include "../Headers/PLayer.h"

void Player::Move(sf::Vector2f ang) {
	//std::cout << ang.x << ' ' << ang.y << ' ' << pos.x << ' ' << pos.z << std::endl;
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


void Player::Update()
{
    /*
    if ((int)pos.x <= terrain->getSize().x && pos.x >= 0 && (int)pos.z <= terrain->getSize().y && pos.z >= 0)
        pos.y = terrain->getHeight(sf::Vector2f(pos.x, pos.z)) + h;
    else
    {
        pos.x = terrain->getSize().x / 2;
        pos.z = terrain->getSize().y / 2;
    }*/
}

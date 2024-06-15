#include <iostream>



void drawCube(Vector3<float> pos, GLuint _texture[], float size) {
    glTranslatef(pos.x, pos.y, pos.z);

    glBindTexture(GL_TEXTURE_2D, _texture[0]);
    glBegin(GL_QUADS);
    //front
    glTexCoord2f(0, 0);   glVertex3f(-size, -size, -size);
    glTexCoord2f(1, 0);   glVertex3f(size, -size, -size);
    glTexCoord2f(1, 1);   glVertex3f(size, size, -size);
    glTexCoord2f(0, 1);   glVertex3f(-size, size, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, _texture[1]);
    glBegin(GL_QUADS);
    //back
    glTexCoord2f(0, 0); glVertex3f(size, -size, size);
    glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
    glTexCoord2f(1, 1); glVertex3f(-size, size, size);
    glTexCoord2f(0, 1); glVertex3f(size, size, size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, _texture[2]);
    glBegin(GL_QUADS);
    //left
    glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
    glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
    glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
    glTexCoord2f(0, 1); glVertex3f(-size, size, size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, _texture[3]);
    glBegin(GL_QUADS);
    //right
    glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
    glTexCoord2f(1, 0); glVertex3f(size, -size, size);
    glTexCoord2f(1, 1); glVertex3f(size, size, size);
    glTexCoord2f(0, 1); glVertex3f(size, size, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, _texture[4]);
    glBegin(GL_QUADS);
    //bottom
    glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
    glTexCoord2f(1, 0); glVertex3f(size, -size, size);
    glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
    glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, _texture[5]);
    glBegin(GL_QUADS);
    //top  		
    glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
    glTexCoord2f(1, 0); glVertex3f(size, size, -size);
    glTexCoord2f(1, 1); glVertex3f(size, size, size);
    glTexCoord2f(0, 1); glVertex3f(-size, size, size);
    glEnd();

    glTranslatef(-pos.x, -pos.y, -pos.z);
}


class Player {
    Vector3<float> pos;
    float speed = .05;
    float h;
    bool onground;
public:
    Vector3f GetPos() {
        return pos;
    }
    float GetSpeed() {
        return speed;
    }
    bool onGround;
    void Move(Vector2f ang) {
        float dx = 0, dz = 0;
        if (Keyboard::isKeyPressed(Keyboard::W))	
		{
		     dx = sin(ang.x/180*M_PI) * -speed; 
		     dz = cos(ang.x/180*M_PI) * -speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::S))	
		{
		    dx = sin(ang.x/180*M_PI) * speed;
		    dz = cos(ang.x/180*M_PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::D))		
		{
		    dx = sin((ang.x+90)/180*M_PI) * speed;	
		    dz = cos((ang.x+90)/180*M_PI) * speed;
		}

		if (Keyboard::isKeyPressed(Keyboard::A))	
		{
		    dx = sin((ang.x-90)/180*M_PI) * speed;	
		    dz = cos((ang.x-90)/180*M_PI) * speed;
		}
        pos.x += dx;
        pos.z += dz;

        
    }

    void Update(Image *terrain, float bias) {

        if((int)pos.x <= terrain->getSize().x && pos.x >= 0 && (int)pos.z <= terrain->getSize().y && pos.z >= 0)
            pos.y = terrain->getPixel(  (int)pos.x*10, (int)pos.z*10 ).r / 15 + bias + h;
        else
        {
            pos.x = terrain->getSize().x / 2;
            pos.z = terrain->getSize().y / 2;
        }
        

    }


    Player(Vector3f _pos, float _h) {
        pos = _pos;
        h = _h;
    }
    
};
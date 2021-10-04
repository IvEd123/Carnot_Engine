#pragma once
#include <array>

class Object {
protected:
	Vector3f pos;
    Vector3f rot;
public:
	virtual void Draw(void) = 0;
	Vector3f GetPos() { return pos; }
	void SetPos(Vector3f _pos) { pos = _pos; }
    Vector3f GetRot() { return rot; }
    void SetRot(Vector3f _rot) { rot = _rot; }
};



class Cube : public Object {
private:
    float size;
    
public:
    Cube(Vector3f _pos, Vector3f _rot, float _size, GLuint *_texture) {
        pos = _pos;
        rot = _rot;
        size = _size;
        texture = _texture;
    }

    GLuint* texture;

    void SetSize(float s) { size = s/2;    }
    float GetSize(void) { return size*2; }


    void Draw() override {

        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(rot.x, 1, 0, 0);
        glRotatef(rot.y, 0, 1, 0);
        glRotatef(rot.z, 0, 0, 1);

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glBegin(GL_QUADS);
        //front
        glTexCoord2f(0, 0);   glVertex3f(-size, -size, -size);
        glTexCoord2f(1, 0);   glVertex3f(size, -size, -size);
        glTexCoord2f(1, 1);   glVertex3f(size, size, -size);
        glTexCoord2f(0, 1);   glVertex3f(-size, size, -size);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBegin(GL_QUADS);
        //back
        glTexCoord2f(0, 0); glVertex3f(size, -size, size);
        glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
        glTexCoord2f(1, 1); glVertex3f(-size, size, size);
        glTexCoord2f(0, 1); glVertex3f(size, size, size);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glBegin(GL_QUADS);
        //left
        glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
        glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
        glTexCoord2f(0, 1); glVertex3f(-size, size, size);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glBegin(GL_QUADS);
        //right
        glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
        glTexCoord2f(1, 0); glVertex3f(size, -size, size);
        glTexCoord2f(1, 1); glVertex3f(size, size, size);
        glTexCoord2f(0, 1); glVertex3f(size, size, -size);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glBegin(GL_QUADS);
        //bottom
        glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
        glTexCoord2f(1, 0); glVertex3f(size, -size, size);
        glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
        glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glBegin(GL_QUADS);
        //top  		
        glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
        glTexCoord2f(1, 0); glVertex3f(size, size, -size);
        glTexCoord2f(1, 1); glVertex3f(size, size, size);
        glTexCoord2f(0, 1); glVertex3f(-size, size, size);
        glEnd();


        glRotatef(-rot.z, 0, 0, 1);
        glRotatef(-rot.y, 0, 1, 0);
        glRotatef(-rot.x, 1, 0, 0);
        glTranslatef(-pos.x, -pos.y, -pos.z);

    }

};


class Plane : public Object {
private:
    Vector2f size;

public:
    GLuint* texture;
    void SetSize(float a, float b) { size.x = a; size.y = b; }
    Vector2f GetSize(void) { return size; }
    void Draw() {
        DrawQuad(pos, size, rot, texture);
    }
};



class Terrain : public Object {
private:
    Image* heightmap;
    GLuint* texture;
    Vector3f pos;
    Vector2i res;
    Vector2f size;
    float scale;
public:
    Terrain(Vector3f _pos, Vector2f _size, Vector2i _res, GLuint* _texture, Image* _heightmap, float _scale) {
        pos = _pos;
        res = _res;
        size = _size;
        scale = _scale;
        texture = _texture;
        heightmap = _heightmap;

    }

    void Draw();

    float getHeight(Vector2f _pos) {

        int i = ((_pos.x-pos.x) / size.x) * heightmap->getSize().x;
        int j = ((_pos.y-pos.z) / size.y) * heightmap->getSize().y;

        return (float)(heightmap->getPixel(i, j).r) * scale;
    }

    Vector2f getSize() {
        return size;
    }
};


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

    void Update(Terrain *terrain, float bias) {

        if ((int)pos.x <= terrain->getSize().x && pos.x >= 0 && (int)pos.z <= terrain->getSize().y && pos.z >= 0)
            pos.y = terrain->getHeight(Vector2f(pos.x, pos.z)) + bias + h;
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

#include "../Headers/header.h"

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

GLuint LoadTexture(const char* name){
    sf::Image image;
    if (!image.loadFromFile(name))
        return EXIT_FAILURE;

    image.flipVertically();

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return texture;
}

void DrawQuad(Vector3f pos, Vector3f rot, Vector3f* points, GLuint* texture) {

    Vector3f N = Vector3f(pl.GetPos().x - sin(angle.x / 180 * M_PI), pl.GetPos().y + tan(angle.y / 180 * M_PI), pl.GetPos().z - cos(angle.x / 180 * M_PI));
    Vector3f P = pos - pl.GetPos();

    float dot = N.x * P.x + N.y * P.y + N.z * P.z;

    if (dot >= 0) {

        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(rot.x, 1.f, 0.f, 1.f);
        glRotatef(rot.y, 0.f, 1.f, 0.f);
        glRotatef(rot.z, 0.f, 0.f, 1.f);

        glBindTexture(GL_TEXTURE_2D, *texture);
        glBegin(GL_QUADS);

        glTexCoord2f(0, 0); glVertex3f(points[0].x, points[0].y, points[0].z);
        glTexCoord2f(1, 0); glVertex3f(points[1].x, points[1].y, points[1].z);
        glTexCoord2f(1, 1); glVertex3f(points[2].x, points[2].y, points[2].z);
        glTexCoord2f(0, 1); glVertex3f(points[3].x, points[3].y, points[3].z);

        glEnd();

        glRotatef(-rot.z, 0, 0, 1);
        glRotatef(-rot.y, 0, 1, 0);
        glRotatef(-rot.x, 1, 0, 0);
        glTranslatef(-pos.x, -pos.y, -pos.z);

    }
}



void DrawQuad(Vector3f pos, Vector2f size, Vector3f rot, GLuint* texture) {


    Vector3f points[4]; {
        points[0] = Vector3f(0, 0, 0);
        points[1] = Vector3f(size.x, 0, 0);
        points[2] = Vector3f(size.x, 0, size.y);
        points[3] = Vector3f(0, 0, size.y);
    }

    DrawQuad(pos, rot, points, texture);
}


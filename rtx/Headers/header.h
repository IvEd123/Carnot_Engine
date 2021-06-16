#pragma once
#include <SFML/OpenGL.hpp>
#include <SFML/System/Vector3.hpp>
#define GL_CLAMP_TO_EDGE 0x812F

using namespace sf;

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar){}

GLuint LoadTexture(const char* name){}

void DrawQuad(Vector3f pos, Vector3f rot, Vector3f* points, GLuint* texture) {}


void DrawQuad(Vector3f pos, Vector2f size, Vector3f rot, GLuint* texture) {}


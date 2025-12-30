#pragma once
#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <gl/GLU.h>
#include <OBJ_Loader.h>

#include "../object/pLayer.h"
#include "../material/material.h"
#include "../object/geometricObject.h"

#define M_PI 3.1415926535897932384626433832795

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

glm::vec3 ConvertSFML2GLM(sf::Vector3f vec);

GLuint loadTexture(const GLchar* path);

GLuint createFrameBuffer(int height, int width, GLuint *depghstencil, GLuint *colorBuff);

GLuint createFrameBuffer(int height, int width);

void bindTexture(const char* path, const char* name, GLuint* handler, Material* material, int i);

void OBJLoader(std::string&, GeometricObject* object);

void OBJLoader_v(std::string& path, GeometricObject* object);

void OBJLoaderLegacy(const char* path, GeometricObject* object);

int loadShader(GLenum type, const GLchar* path);

enum GeometryType{
	CUBE,
	TERRAIN,
	MESH,
	PLANE
};

#endif
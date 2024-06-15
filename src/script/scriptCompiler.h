#pragma once
#ifndef SCRIPTCOMPILER_H
#define SCRIPTCOMPILER_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdint.h>
#include <fstream>

#include <SFML/Graphics.hpp>

class GeometricObject;


const enum KEYWORDS {
	UPDATE_F, START_F, INT_VAR, VEC3, MOVE, MOVETO, BRACKET_L, BRACKET_R, FLOAT_VAR, ROTATE, ROTATETO
};

static std::map<std::string, uint8_t> keywords = {		// map with keywords
	{"int", INT_VAR},
	{"vec3", VEC3},
	{"float", FLOAT_VAR},
	{"update", UPDATE_F},
	{"start", START_F},
	{"move", MOVE},
	{"moveto", MOVETO},
	{"rotate", ROTATE},
	{"rotateto", ROTATETO},
	{"{", BRACKET_L}, 
	{"}", BRACKET_R}
};

static std::map<uint8_t, uint8_t> arguments = {			// number of arguments for every instruction
	{INT_VAR, 1},
	{VEC3, 3},
	{MOVE, 3},
	{MOVETO, 3}, 
	{ROTATE, 3},
	{ROTATETO, 3}, 
	{FLOAT_VAR, 1}
};	
											
class ScriptHandler {
public:
	GeometricObject*						object;						// pointer to parent object
	std::map <std::string, int>				int_var_names;				// variable names and indecies - integer
	std::map <std::string, int>				float_var_names;			// variable names and indecies - float
	std::string								path;						// path to script file
	std::vector<int>						int_variables;				// buffer with integers
	std::vector<float>						float_variables;			// buffer with floats
	std::vector<uint8_t>					update_inst;				// instructions in update function
	std::vector<uint8_t>					start_inst;					// instructions in start function
	int										bc;							// bracket counter;
	void									Update();					// update function
	void									Start();					// start function
private:
	void									move(sf::Vector3f v);		// add vector to object position
	void									moveto(sf::Vector3f v);		// changes object position
	void									rotate(sf::Vector3f v);		// add vector to object rotation
	void									rotateto(sf::Vector3f v);	// changes object rotation
};

class ScriptCompiler{
public:
	ScriptHandler							LoadScript(std::string path, GeometricObject *obj);
};

#endif
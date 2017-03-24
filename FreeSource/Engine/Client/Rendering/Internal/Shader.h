#pragma once


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#ifndef GRAPHICS_INCLUDED
	#define GLEW_STATIC
	#include "GL/glew.h"
	#define GLFW_INCLUDE_GLU
	#include "GLFW/glfw3.h"

#else

	#define GRAPHICS_INCLUDED
#endif


#include "../../../Util/Debug.h"

class Shader
{
public:

	GLuint programID;

	ErrorType status = ErrorType::NO_ERROR;

	// Load the shader
	void readFromFile(const char* vertexPath, const char* fragmentPath);


	// Activate the shader for following GL calls
	void use();

	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
};


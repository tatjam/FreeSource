#include "Shader.h"



void Shader::readFromFile(const char* vertexPath, const char* fragmentPath)
{
	const char *vertexCode, *fragmentCode;
	std::ifstream vertexCodeStream, fragmentCodeStream;

	// Enable exceptions
	vertexCodeStream.exceptions(std::ifstream::badbit);
	fragmentCodeStream.exceptions(std::ifstream::badbit);

	std::string vs, fs;
	try
	{
		vertexCodeStream.open(vertexPath);
		fragmentCodeStream.open(fragmentPath);

		if (!(vertexCodeStream.good() && vertexCodeStream.good()))
		{
			Debug::error("Could not read/open shader file");
			status = ErrorType::IO_ERROR;
			return;
		}

		std::stringstream vCodeWriter, fCodeWriter;
		
		vCodeWriter << vertexCodeStream.rdbuf();
		fCodeWriter << fragmentCodeStream.rdbuf();

		vertexCodeStream.close();
		fragmentCodeStream.close();

		// Get string

		vs = vCodeWriter.str();
		fs = fCodeWriter.str();

		// Get the char array
		vertexCode = vs.c_str();
		fragmentCode = fs.c_str();

		// (cannot be joined as .str() retuns a heap string)
	}
	catch(std::ifstream::failure e)
	{
		Debug::error("Could not read/open shader file");
		status = ErrorType::IO_ERROR;
		return;
	}

	// Compile the shaders
	
	GLuint vertex, fragment;
	GLint success;
	char infoLog[512];

	// Vertex shader

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);

	// Compiler errors

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		Debug::error("Could not compile vertex shader, error log: \n");
		Debug::error(infoLog);
	}

	// Same for fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);

	// Compiler errors

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		Debug::error("Could not compile fragment shader, error log: \n");
		Debug::error(infoLog);
	}

	// Link the program
	// Shader Program
	this->programID = glCreateProgram();
	glAttachShader(this->programID, vertex);
	glAttachShader(this->programID, fragment);
	glLinkProgram(this->programID);
	// Print linking errors if any
	glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
		Debug::error("Could not link shader, error log: \n");
		Debug::error(infoLog);
	}


	// We dont need the shaders anymore
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(this->programID);
}


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	readFromFile(vertexPath, fragmentPath);
}


Shader::Shader()
{
}


Shader::~Shader()
{
}

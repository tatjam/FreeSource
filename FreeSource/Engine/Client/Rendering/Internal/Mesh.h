#pragma once



#ifndef GRAPHICS_INCLUDED

#define GRAPHICS_INCLUDED
#define GLEW_STATIC
#include "GL/glew.h"
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"

#endif


#ifndef LOGURU_INCLUDED
#define LOGURU_INCLUDED
#include "../../../Util/loguru.hpp"
#endif


#include "Shader.h"
#include "LightScene.h"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


// TODO: For some reason Mesh cannot import loguru. WTF?
//#include "../../../Util/loguru.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::vector;
using std::stringstream;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bittangent;
};

struct Texture
{
	GLuint id;
	std::string type;
};

struct MaterialData
{
	float specularPower = 1.0f;
	float emissionPower = 1.0f;
	float shininess = 32.0f;

	glm::vec2 diffuseOffset;
	glm::vec2 specularOffset;
	glm::vec2 emissiveOffset;

	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 emissionColor;

};

class Mesh
{
public:

	MaterialData materialData;

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	std::string name;

	void build(vector<Vertex> v, vector<GLuint> i, vector<Texture> t);

	void draw(Shader shader, LightScene lScene, glm::mat4 transform, glm::mat4 world, glm::mat4 persp, GLuint shadowMapID);
	void drawShadow(Shader shader, glm::mat4 light, glm::mat4 transform);

	Mesh(vector<Vertex> v, vector<GLuint> i, vector<Texture> t, std::string name);
	Mesh();
	~Mesh();

private:

	GLuint VAO, VBO, EBO;

	void setup();
};


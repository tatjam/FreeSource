#pragma once

#include "glm/glm.hpp"
#ifndef GRAPHICS_INCLUDED
#define GLEW_STATIC
#include "GL/glew.h"
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"

#else

#define GRAPHICS_INCLUDED
#endif

#include "Shader.h"
#include "LightScene.h"

#include <string>
#include <vector>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using std::vector;
using std::stringstream;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	GLuint id;
	string type;
};

struct MaterialData
{
	float specularPower = 1.0f;
	float emissionPower = 1.0f;

	glm::vec2 diffuseOffset;
	glm::vec2 specularOffset;
	glm::vec2 emissiveOffset;

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
	~Mesh();

private:

	GLuint VAO, VBO, EBO;

	void setup();
};


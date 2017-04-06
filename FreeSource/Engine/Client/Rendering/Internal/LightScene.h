#pragma once


#include <vector>
#include <glm/glm.hpp>

using std::vector;

#define MAX_LIGHTS 3

struct PointLight 
{
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

};


struct DirLight 
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct LightScene
{
	PointLight pointLights[MAX_LIGHTS];
	DirLight dirLight;

	bool dirLightEnabled = false;
	int pointLightCount = 0;

	glm::mat4 lMatrix;
};


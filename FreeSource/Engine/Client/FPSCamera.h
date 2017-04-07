#pragma once
#include "glm/glm.hpp"
#include <glm/gtx/rotate_vector.hpp>

class FPSCamera
{

private:

public:

	glm::vec3 pos;
	// Our up axis, which can be redefined. !!MUST BE NORMALIZED!!
	glm::vec3 up = glm::vec3(0, 1, 0);  
	// Rotation up-down (limited from -89.9f to 89.9f)
	float azimuth = 0.0f;
	// Rotation left-right
	float polar = 0.0f; 

	void lookTo(glm::vec3 loc)
	{

	}

	// Gets vector along camera view
	glm::vec3 getForwardVector()
	{
		glm::vec3 base;
		base.x = cos(glm::radians(polar)) * cos(glm::radians(azimuth));
		base.y = sin(glm::radians(azimuth));
		base.z = sin(glm::radians(polar)) * cos(glm::radians(azimuth));
		return base;
	}

	// Gets vector towards the right of camera view
	glm::vec3 getRightVector()
	{
		glm::vec3 f = getForwardVector();
		return glm::normalize(glm::cross(glm::normalize(f), up));
	}

	// Gets vector towards top of the camera view
	glm::vec3 getUpVector()
	{
		glm::vec3 f = getForwardVector();
		return glm::normalize(glm::cross(glm::normalize(f), -getRightVector()));
	}

	glm::vec3 getLookAtTarget()
	{
		glm::vec3 out;


		//out = glm::rotate(out, glm::radians(azimuth), up);
		if (azimuth > 89.9f)
		{
			azimuth = 89.9f;
		}
		if (azimuth < -89.9f)
		{
			azimuth = -89.9f;
		}

		

		// Norm contains our point of circle along dir(0, base)
		//glm::vec3 norm;
		//orm.x = ;
		//norm.y = sin(glm::radians(azimuth));

		glm::vec3 base = getForwardVector();

		out = base;

		out += pos;
		return out;
	}

	FPSCamera()
	{
		
	}

	~FPSCamera()
	{

	}
};
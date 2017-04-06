#pragma once

#include "Internal/Shader.h"
#include "Internal/LightScene.h"

class Renderer;

class Drawable
{
public:

	void start()
	{

	}


	virtual void drawShadow(glm::mat4 light, Shader* shadowShader)
	{

	}

	virtual void draw(glm::mat4 world, glm::mat4 proj, LightScene lscene, GLuint prog)
	{
		return;
	}

	Drawable()
	{

	}
	~Drawable()
	{

	}
};


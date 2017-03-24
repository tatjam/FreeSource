#pragma once
#include "Model.h"
#include "Internal/LightScene.h"

#include <vector>


struct Camera
{
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;

	float close;
	float far;

	float fov;
};

class SceneRenderer
{
public:


	std::vector<Drawable*> drawables;

	Camera camera;

	bool renderShadows = true;

	LightScene lScene;

	glm::mat4 view;
	glm::mat4 proj;

	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, int width, int height);

	void start(int WIDTH, int HEIGHT);

	void draw(int WIDTH, int HEIGHT);

	SceneRenderer();
	~SceneRenderer();

	GLuint depthMap;

	glm::vec3 dLightPos = glm::vec3(-2.0f, 4.0f, -1.0f);

private:

	GLuint framebuffer;
	GLuint texColorBuffer;
	GLuint rbo;
	GLuint quadVAO, quadVBO;


	GLuint depthMapFBO;


	void createFramebuffer(int WIDTH, int HEIGHT);

	Shader frameShader;
	Shader shadowShader;

	int doEffect = 1;


	float shadowNPlane = 1.0f;
	float shadowFPlane = 90.5f;

	const GLuint shadowWidth = 1024, shadowHeight = 1024;


};


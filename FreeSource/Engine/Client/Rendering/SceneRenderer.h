#pragma once
#include "Model.h"
#include "Internal/LightScene.h"

#include <vector>

#include <glm/gtx/rotate_vector.hpp>

struct Camera
{
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;

	float close;
	float far;

	float fov;
	float exposure;
	float contrast;
};

struct DebugObject
{
	GLfloat* data;
	int data_size;
	GLuint VBO, VAO;
	int length;
	int vertexCount;
	int type;
	float duration;
};

class SceneRenderer
{
public:


	std::vector<Drawable*> drawables;

	std::vector<DebugObject> debug;


	Camera camera;

	bool renderShadows = true;

	LightScene lScene;

	glm::mat4 view;
	glm::mat4 proj;

	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, int width, int height);

	void start(int WIDTH, int HEIGHT);

	void draw(int WIDTH, int HEIGHT, float dt);

	// Debug drawing routine. Draws a line. EXTREMELY SLOW
	void drawDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, float duration);

	// Debug drawing routine. Draws a pointed arrow. EXTREMELY SLOW
	void drawDebugArrow(glm::vec3 start, glm::vec3 end, glm::vec3 color, float duration);

	// Debug drawing routine. Draws a pointed arrow with 2 colors. EXTREMELY SLOW
	void drawDebugArrow(glm::vec3 start, glm::vec3 end, glm::vec3 color, glm::vec3 headColor, float duration);

	// Debug drawing routine. Draws a point. EXTREMELY SLOW
	void drawDebugPoint(glm::vec3 pos, glm::vec3 color, float duration);

	SceneRenderer();
	~SceneRenderer();

	GLuint depthMap;

	glm::vec3 dLightPos = glm::vec3(-2.0f, 4.0f, -1.0f);

	GLuint skybox = 0;

private:

	GLuint framebuffer;
	GLuint texColorBuffer;
	GLuint rbo;
	GLuint quadVAO, quadVBO;

	GLuint skyVAO, skyVBO;

	GLuint depthMapFBO;


	void createFramebuffer(int WIDTH, int HEIGHT);

	Shader frameShader;
	Shader shadowShader;
	Shader debugShader;
	Shader skyShader;

	int doEffect = 1;


	float shadowNPlane = 1.0f;
	float shadowFPlane = 90.5f;

	const GLuint shadowWidth = 512, shadowHeight = 512;


};


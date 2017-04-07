#include <iostream>
#include <cmath>
#include <math.h>
#include <thread>     


#define GLEW_STATIC
#include "GL/glew.h"
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"


#include "SOIL.h"

#define GRAPHICS_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#ifndef LOGURU_INCLUDED
#define LOGURU_INCLUDED
#define LOGURU_IMPLEMENTATION 1
#include "Engine/Util/loguru.hpp"
#endif


#include "Engine/Client/AssetManager.h"

#include "Engine/Client/Rendering/Internal/LModel.h"

#include "Engine/Client/Rendering/Internal/Shader.h"
#include "Engine/Client/Rendering/Internal/Mesh.h"

#include "Engine/Util/Transform.h"

#include "Engine/Client/Rendering/SceneRenderer.h"

#define TERM_INCLUDE_DEFINITION
#include "Engine/Util/tinyterm.h"

#include "Engine/Client/FPSCamera.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//
GLfloat dt = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame





// Dont delete this line!
#undef far


int main(int argc, char *argv[])
{
	// Start logger (Thx loguru :D)
	loguru::init(argc, argv);
	loguru::add_file("output.log", loguru::Truncate, loguru::Verbosity_MAX);
	loguru::add_file("output_readable.log", loguru::Truncate, loguru::Verbosity_INFO);
	loguru::g_stderr_verbosity = 1;

	//LOG_SCOPE_F(INFO, "Initialization");

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	LOG_F(INFO, "GLFW and OpenGL initialized");

	// OpenGL options

	LOG_F(INFO, "Asset loading");

	AssetManager assetManager = AssetManager();

	assetManager.loadModel("Resource/lunar_lander.obj", "duck");
	assetManager.loadModel("Resource/medieval_house.obj", "mapDemo");

	// Build and compile our shader program
	Shader lightingShader("Resource/Shader/test_shader.vert", "Resource/Shader/test_shader.frag");
	Shader lampShader("Resource/Shader/basic_unlit.vert", "Resource/Shader/basic_unlit.frag");

	LModel testLModel = LModel("Resource/q3rocket.obj", &assetManager);
	Model testModel = Model(assetManager.getModel("duck"), &lightingShader, &assetManager);
	Model houseModel = Model(assetManager.getModel("mapDemo"), &lightingShader, &assetManager);

	glfwSwapInterval(0);

	SceneRenderer renderer = SceneRenderer();
	renderer.start(WIDTH, HEIGHT);

	renderer.camera.close = 0.1f;
	renderer.camera.far = 100.0f;
	renderer.camera.fov = 70.0f;
	renderer.camera.target = glm::vec3(0.0f, 0.0f, 0.0f);
	renderer.camera.position = glm::vec3(3.0f, 3.0f, -8.0f);
	renderer.camera.up = glm::vec3(0, 1, 0);

	renderer.drawables.push_back(&testModel);
	renderer.drawables.push_back(&houseModel);

	testModel.transform.scaling = glm::vec3(1.3f, 1.3f, 1.3f);
	testModel.transform.setEulerAngles(0.0f, 0.0f, 0.0f);
	testModel.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	testModel.transform.buildMatrix();

	houseModel.transform.scaling = glm::vec3(0.8f, 0.8f, 0.8f);
	houseModel.transform.position = glm::vec3(5.0f, -4.0f, 0.0f);
	houseModel.transform.setEulerAngles(0.0f, 0.0f, 0.0f);
	houseModel.transform.buildMatrix();

	renderer.lScene.dirLightEnabled = true;
	renderer.lScene.dirLight.direction = glm::vec3(1, -0.5f, 1);
	renderer.lScene.dirLight.diffuse = glm::vec3(4.0f, 2.3f, 2.3f);
	renderer.lScene.dirLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	renderer.lScene.dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

	float t = 0.0f;
	float dt = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f;  	// Time of last frame

	glfwSwapInterval(0);

	float radius = 5.0f;

	testModel.createMeshMap();

	std::vector<Mesh*> meshes = testModel.getAllMeshes();

	LOG_SCOPE_F(INFO, "Game loop");

	unsigned int frameNum = 0;



	float time = 0.0f;
	renderer.camera.exposure = 1.5f;


	assetManager.loadTexture("Resource/detail_font.bmp", "font", 3, true, false);

	LOG_F(INFO, "PRE_CREATE_STUFF");

	termScreen scr = term_create_screen(20, 20);
	termFont font = term_load_font((char*)assetManager.getTexture("font")->data, 
		assetManager.getTexture("font")->width * 
		assetManager.getTexture("font")->height * 
		assetManager.getTexture("font")->sizePerPixel, 
		assetManager.getTexture("font")->width, 32, 32, 0);
	termImage img = term_create_image(20 * font.width, 20 * font.height);

	LOG_F(INFO, "POST_CREATE_STUFF");

	FPSCamera camera = FPSCamera();

	bool d_enable = false;

	float cameraSpeed = 1.0f;

	bool inside_camera = false;

	std::vector<TextureData*> cubeSides;
	cubeSides.push_back(assetManager.loadTextureData("Resource/sky_extra/red_rt.tga", "arch3_+x"));
	cubeSides.push_back(assetManager.loadTextureData("Resource/sky_extra/red_lf.tga", "arch3_-x"));
	cubeSides.push_back(assetManager.loadTextureData("Resource/sky_extra/red_up.tga", "arch3_+y"));
	cubeSides.push_back(assetManager.loadTextureData("Resource/sky_extra/red_dn.tga", "arch3_-y"));
	cubeSides.push_back(assetManager.loadTextureData("Resource/sky_extra/red_bk.tga", "arch3_+z"));
	cubeSides.push_back(assetManager.loadTextureData("Resource/sky_extra/red_ft.tga", "arch3_-z"));

	GLuint sky = assetManager.createCubemap(cubeSides.data(), "sky");

	renderer.skybox = sky;

	float edgeSmooth = 0.0f;

	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();


		for (int i = 0; i < 20 * 20; i++)
		{
			scr.chars[i].character = (int)(i + t * 10) % 255;
			scr.chars[i].fr = (sin(t) + 1.0f) * 128.0f;
			scr.chars[i].fg = (cos(t) + 1.0f) * 128.0f;
			scr.chars[i].fb = (tan(t * i / 100.0f) + 1.0f) * 128.0f;
			scr.chars[i].bb = (sin(t) + 1.0f) * 128.0f;
			scr.chars[i].br = (cos(t) + 1.0f) * 128.0f;
			scr.chars[i].bg = (tan(t * i / 100.0f) + 1.0f) * 128.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		{
			edgeSmooth -= 1.0f * dt;
			LOG_F(INFO, "EDGE SMOOTH: %f", edgeSmooth);
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			edgeSmooth += 1.0f * dt;
			LOG_F(INFO, "EDGE SMOOTH: %f", edgeSmooth);
		}

		for (int i = 0; i < assetManager.getModel("mapDemo")->meshes.size(); i++)
		{
			assetManager.getModel("mapDemo")->meshes[i].materialData.edgeSmooth = edgeSmooth;
			//LOG_F(INFO, "EDGE_SMOOTH: %f", sin(t / 5) * 5.0f);
		}

		/*
		if (frameNum % 20 == 0)
		{
			term_render_image(&scr, &img, &font, 1);

			term_upload_gl_image(&img, assetManager.textures["Resource/AluminumOxy.png"]);
		}
		*/




		// Calculate dt of current frame
		double currentFrame = glfwGetTime();
		dt = currentFrame - lastFrame;
		lastFrame = currentFrame;


		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			camera.azimuth += 90.0f * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			camera.azimuth -= 90.0f * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			camera.polar += 90.0f * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			camera.polar -= 90.0f * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.pos += camera.getForwardVector() * cameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.pos += -camera.getForwardVector() * cameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.pos += camera.getRightVector() * cameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.pos += -camera.getRightVector() * cameraSpeed * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			cameraSpeed += 1.0f * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			cameraSpeed -= 1.0f * dt;
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			inside_camera = false;
			d_enable = true;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			inside_camera = true;
			d_enable = false;
		}


		if (d_enable)
		{
			renderer.drawDebugArrow(camera.pos, camera.getLookAtTarget(), glm::vec3(1, 0.3f, 0.3f), glm::vec3(1, 0, 0), 0);
			renderer.drawDebugArrow(camera.pos, camera.pos + camera.getRightVector(), glm::vec3(1, 0.3f, 0.3f), glm::vec3(0, 1, 0), 0);
			renderer.drawDebugArrow(camera.pos, camera.pos + camera.getUpVector(), glm::vec3(1, 0.3f, 0.3f), glm::vec3(0, 0, 1), 0);

			renderer.drawDebugPoint(camera.getLookAtTarget(), glm::vec3(1, 1, 1), 1.0f);
		}

		if (inside_camera)
		{
			renderer.camera.position = camera.pos;
			renderer.camera.target = camera.getLookAtTarget();
		}
		else
		{
			renderer.camera.position = glm::vec3(sin(t / 2) * radius, 3.0f, cos(t / 2) * radius);
			renderer.camera.target = glm::vec3(-2.0f, 0.0f, 0.0f);
		}

		//renderer.lScene.dirLight.direction = glm::vec3(sin(t), cos(t), sin(t) * cos(t));

		/*testModel.transform.rotateEuler(0, 45.0f * dt, 5.0f * dt);
		testModel.transform.buildMatrix();*/


		for (int i = 0; i < (int)meshes.size(); i++)
		{
			meshes[i]->materialData.emissiveOffset = glm::vec2(sin(t) * 1, cos(t) * 1);
			//meshes[i]->materialData.emissionPower = (sin(t) + 1) / 4;
			meshes[i]->materialData.emissionPower = 0.0f;
			//meshes[i]->materialData.diffuseOffset = glm::vec2(sin(t) * 0.2f, cos(t) * 0.2f);
		}

		renderer.draw(WIDTH, HEIGHT, dt);

		// Swap the screen buffers
		glfwSwapBuffers(window);

		if (frameNum % 10 == 0)
		{
			LOG_F(INFO, "FPS: %f", 1.0f / dt);
		}

		t += dt;
		frameNum++;

	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

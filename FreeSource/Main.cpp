#include <iostream>
#include <cmath>
#include <math.h>



#define GLEW_STATIC
#include "GL/glew.h"
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"

#include "Engine/Util/Debug.h"


#include "SOIL.h"

#define GRAPHICS_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Client/Rendering/Internal/Shader.h"
#include "Engine/Client/Rendering/Internal/Mesh.h"

#include "Engine/Util/Transform.h"

#include "Engine/Client/Rendering/SceneRenderer.h"



// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// 
GLfloat dt = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

#include "Engine/Client/Rendering/Internal/LModel.h"


/*
#include "Engine/Util/rang.hpp"
*/

#include "Engine/Util/Logger/Logger.h"



// Dont delete this line!
#undef far




int main()
{


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

	// OpenGL options


	AssetManager::getInstance().loadModel("Resource/q3rocket.obj", "q3rocket");
	AssetManager::getInstance().loadModel("Resource/medieval_house.obj", "house");

	// Build and compile our shader program
	Shader lightingShader("Resource/Shader/test_shader.vert", "Resource/Shader/test_shader.frag");
	Shader lampShader("Resource/Shader/basic_unlit.vert", "Resource/Shader/basic_unlit.frag");

	LModel testLModel = LModel("Resource/q3rocket.obj");
	Model testModel = Model(AssetManager::getInstance().getModel("q3rocket"), &lightingShader);
	Model houseModel = Model(AssetManager::getInstance().getModel("house"), &lightingShader);

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

	testModel.transform.scaling = glm::vec3(0.2f, 0.2f, 0.2f);
	testModel.transform.setEulerAngles(0.0f, 0.0f, 0.0f);
	testModel.transform.buildMatrix();

	houseModel.transform.scaling = glm::vec3(0.8f, 0.8f, 0.8f);
	houseModel.transform.position = glm::vec3(5.0f, -4.0f, 0.0f);
	houseModel.transform.setEulerAngles(0.0f, 0.0f, 0.0f);
	houseModel.transform.buildMatrix();

	renderer.lScene.dirLightEnabled = true;
	renderer.lScene.dirLight.direction = glm::vec3(1, -0.5f, 1);
	renderer.lScene.dirLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	renderer.lScene.dirLight.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	renderer.lScene.dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

	float t = 0.0f;
	float dt = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f;  	// Time of last frame

	glfwSwapInterval(1);

	float radius = 10.0f;

	testModel.createMeshMap();

	std::cout << "There are " << testModel.meshes.size() << " meshes" << std::endl;

	std::vector<Mesh*> meshes = testModel.getAllMeshes();

	//renderer.depthMap = AssetManager::getInstance().loadTexture("Resource/rocketl.png");
	AssetManagerI.loadTexture("Resource/dataTest.png");
	
	/*
	std::cout << "TEXTURE MEM DUMP SIZE: " << AssetManagerI.texturesData["Resource/dataTest.png"].size
		<< ", DATA_SIZE: " << AssetManagerI.texturesData["Resource/dataTest.png"].sizePerPixel << std::endl;

	std::cout << "DATA LOCATION: " << (int)AssetManagerI.texturesData["Resource/dataTest.png"].ptr << std::endl;

	for (int x = 0; x < AssetManagerI.texturesData["Resource/dataTest.png"].w; x++)
	{
		for (int y = 0; y < AssetManagerI.texturesData["Resource/dataTest.png"].h; y++)
		{
			std::cout << AssetManager::getColorString(AssetManagerI.texturesData["Resource/dataTest.png"].getPixel(x, y)) << std::endl;
		}
	}

	for (int i = 0; i < AssetManagerI.getTexture("Resource/dataTest.png")->size * 
		AssetManagerI.getTexture("Resource/dataTest.png")->sizePerPixel;
		i += AssetManagerI.getTexture("Resource/dataTest.png")->sizePerPixel)
	{
		std::cout << "(I: " << i / AssetManagerI.getTexture("Resource/dataTest.png")->sizePerPixel << "), {" << std::endl;
		for (int x = 0; x < AssetManagerI.getTexture("Resource/dataTest.png")->sizePerPixel; x++)
		{
			std::cout << "    " << (int)AssetManagerI.getTexture("Resource/dataTest.png")->ptr[i + x] << std::endl;
		}
		std::cout << "}" << std::endl;

	}

	*/


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Calculate dt of current frame
		float currentFrame = glfwGetTime();
		dt = currentFrame - lastFrame;
		lastFrame = currentFrame;

		renderer.camera.position += glm::vec3(sin(t) * dt * 5, cos(t) * 3 * dt, 0.5f * dt);
		
		renderer.lScene.dirLight.direction = glm::vec3(sin(t), cos(t), sin(t) * cos(t));

		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i]->materialData.emissiveOffset = glm::vec2(sin(t) * 1, cos(t) * 1);
			//meshes[i]->materialData.emissionPower = (sin(t) + 1) / 4;
			meshes[i]->materialData.emissionPower = 0.0f;
			//meshes[i]->materialData.diffuseOffset = glm::vec2(sin(t) * 0.2f, cos(t) * 0.2f);
		}

		renderer.draw(WIDTH, HEIGHT);

		// Swap the screen buffers
		glfwSwapBuffers(window);

		t += 1.f * dt;

	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

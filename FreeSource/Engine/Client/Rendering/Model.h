#pragma once

// LModel should have everything we need included
#include "../../Util/Transform.h"
#include "../../Util/simple_multimap.h"
#include "Drawable.h"
#include "Internal/Mesh.h"
#include "Internal/LModel.h"

class LModel;

// Model used for rendering, it's rendered by the Renderer automatically
class Model : public Drawable
{
public:

	AssetManager* assetManager;

	// Low level mode used for rendering
	LModel* model;
	Shader* shader;

	

	Transform transform;

	// Map with all names and the mesh with that name.
	std::map<std::string, Mesh*> meshes;


	// Load the model with name "name" from the AssetManager
	void create(std::string name);

	void createMeshMap();

	std::vector<std::string> getAllMeshNames();
	std::vector<Mesh*> getAllMeshes();

    void draw(glm::mat4 world, glm::mat4 proj, LightScene lScene, GLuint shadowMapID);
	void drawShadow(glm::mat4 light, Shader* shadowShader);

	void start();

	void load(LModel* model, Shader* shader);
	void load(std::string name, Shader* shader);

	Model();
	Model(LModel* model, Shader* shader, AssetManager* manager);
	Model(std::string name, Shader* shader, AssetManager* manager);
	~Model();
};

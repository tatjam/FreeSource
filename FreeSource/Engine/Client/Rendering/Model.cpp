#include "Model.h"


void Model::draw(glm::mat4 world, glm::mat4 proj, LightScene lScene, GLuint shadowMapID, GLuint sky, glm::vec3 cPos)
{
	model->draw(*shader, lScene, transform.modelMatrix, world, proj, shadowMapID, sky, cPos);
}

void Model::drawShadow(glm::mat4 light, Shader* shadowShader)
{
	model->drawShadow(*shadowShader, light, transform.modelMatrix);
}

void Model::createMeshMap()
{
	meshes = std::map<std::string, Mesh*>();

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		std::string ps = model->meshes[i].name;
		if (ps == "")
		{
			ps = std::string("NO_NAME." + i);
		}
		meshes[ps] = &model->meshes[i];

	}
}

std::vector<std::string> Model::getAllMeshNames()
{
	vector<std::string> out;
	for (map<std::string, Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		out.push_back(it->first);
	}
	return out;
}

std::vector<Mesh*> Model::getAllMeshes()
{
	vector<Mesh*> out;
	for (map<std::string, Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		out.push_back(it->second);
	}
	return out;
}

Model::Model(LModel* model, Shader* shader, AssetManager* manager)
{
	this->model = model;
	this->shader = shader;
	this->assetManager = manager;
}

void Model::load(LModel* model, Shader* shader)
{
	this->model = model;
	this->shader = shader;
}

void Model::load(std::string name, Shader* shader)
{
	this->model = assetManager->models[name];
	this->shader = shader;
}

Model::Model(std::string name, Shader* shader, AssetManager* manager)
{
	assetManager = manager;
	load(name, shader);
}

Model::Model()
{
}


Model::~Model()
{
}

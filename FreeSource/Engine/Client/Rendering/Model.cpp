#include "Model.h"


void Model::draw(glm::mat4 world, glm::mat4 proj, LightScene lScene, GLuint shadowMapID)
{
	model->draw(*shader, lScene, transform.modelMatrix, world, proj, shadowMapID);
}

void Model::drawShadow(glm::mat4 light, Shader* shadowShader)
{
	model->drawShadow(*shadowShader, light, transform.modelMatrix);
}

void Model::createMeshMap()
{
	meshes = std::map<std::string, Mesh*>();

	for (int i = 0; i < model->meshes.size(); i++)
	{
		std::string ps = model->meshes[i].name;
		if (ps == "")
		{
			ps = std::string("NO_NAME." + i);
		}
		meshes[ps] = &model->meshes[i];
	}
}

std::vector<string> Model::getAllMeshNames()
{
	vector<string> out;
	for (map<string, Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		out.push_back(it->first);
	}
	return out;
}

std::vector<Mesh*> Model::getAllMeshes()
{
	vector<Mesh*> out;
	for (map<string, Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		out.push_back(it->second);
	}
	return out;
}

Model::Model(LModel* model, Shader* shader)
{
	this->model = model;
	this->shader = shader;
}

void Model::load(LModel* model, Shader* shader)
{
	this->model = model;
	this->shader = shader;
}

void Model::load(std::string name, Shader* shader)
{
	this->model = AssetManager::getInstance().models[name];
	this->shader = shader;
}

Model::Model()
{
}


Model::~Model()
{
}

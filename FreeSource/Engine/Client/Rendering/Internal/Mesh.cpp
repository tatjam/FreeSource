#include "Mesh.h"

void Mesh::build(vector<Vertex> v, vector<GLuint> i, vector<Texture> t)
{
	vertices = v;
	indices = i;
	textures = t;
	setup();
}

void Mesh::drawShadow(Shader s, glm::mat4 light, glm::mat4 transform)
{
	glUniformMatrix4fv(glGetUniformLocation(s.programID, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(light));
	glUniformMatrix4fv(glGetUniformLocation(s.programID, "model"), 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::draw(Shader s, LightScene lScene, glm::mat4 transform, glm::mat4 world, glm::mat4 persp, GLuint shadowMapId, GLuint sky, glm::vec3 cameraPos)
{

	bool diffuseLoaded = false;
	bool specularLoaded = false;
	bool emissionLoaded = false;
	bool normalLoaded = false;

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(s.programID, "shadowMap"), 0);
	glBindTexture(GL_TEXTURE_2D, shadowMapId);
	glActiveTexture(0);

	int loadedTextures = 0;

	for (int i = 0; i < (int)textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);

		if (textures[i].type == "diffuse" && !diffuseLoaded)
		{
			diffuseLoaded = true;
			glUniform1i(glGetUniformLocation(s.programID, "material.diffuse"), i + 1);
		}
		else if (textures[i].type == "specular" && !specularLoaded)
		{
			specularLoaded = true;
			glUniform1i(glGetUniformLocation(s.programID, "material.specular"), i + 1);
		}
		else if (textures[i].type == "emissive" && !emissionLoaded)
		{
			emissionLoaded = true;
			glUniform1i(glGetUniformLocation(s.programID, "material.emissive"), i + 1);
		}
		else if (textures[i].type == "normal" && !normalLoaded)
		{
			normalLoaded = true;
			glUniform1i(glGetUniformLocation(s.programID, "material.normalMap"), i + 1);
		}



		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		loadedTextures++;
		//if(diffuseLoaded || specularLoaded || emissionLoaded)


	}


	if (sky != 0)
	{
		glActiveTexture(GL_TEXTURE1 + loadedTextures + 1);
		glUniform1i(glGetUniformLocation(s.programID, "skyboxEnabled"), 1);
		glUniform1i(glGetUniformLocation(s.programID, "skybox"), loadedTextures + 2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, sky);
	}
	else
	{
		glUniform1i(glGetUniformLocation(s.programID, "skyboxEnabled"), 0);
	}

	glUniform3f(glGetUniformLocation(s.programID, "material.reflective"), materialData.reflectiveColor.x,
		materialData.reflectiveColor.y, materialData.reflectiveColor.z);

	glUniform1f(glGetUniformLocation(s.programID, "edgeSmooth"), materialData.edgeSmooth);

	glActiveTexture(0);


	// Load transforms
	// Pass the matrices to the shader
	glUniformMatrix4fv(glGetUniformLocation(s.programID, "model"), 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(glGetUniformLocation(s.programID, "view"), 1, GL_FALSE, glm::value_ptr(world));
	glUniformMatrix4fv(glGetUniformLocation(s.programID, "projection"), 1, GL_FALSE, glm::value_ptr(persp));
	glUniformMatrix4fv(glGetUniformLocation(s.programID, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lScene.lMatrix));

	// Load 0 textures
	if (!diffuseLoaded)
	{
		glUniform1i(glGetUniformLocation(s.programID, "material.diffuseEnabled"), 0);
		glUniform3f(glGetUniformLocation(s.programID, "material.diffuseValue"), 
			materialData.diffuseColor.r, materialData.diffuseColor.g, materialData.diffuseColor.b); 
	}
	else
	{
		glUniform1i(glGetUniformLocation(s.programID, "material.diffuseEnabled"), 1);
	}
	if (!specularLoaded)
	{
		glUniform1i(glGetUniformLocation(s.programID, "material.specularEnabled"), 0);
		glUniform3f(glGetUniformLocation(s.programID, "material.specularValue"),
			materialData.specularColor.r, materialData.specularColor.g, materialData.specularColor.b);

	}
	else
	{
		glUniform1i(glGetUniformLocation(s.programID, "material.specularEnabled"), 1);
	}
	if (!emissionLoaded)
	{
		glUniform1i(glGetUniformLocation(s.programID, "material.emissiveEnabled"), 0);
		glUniform3f(glGetUniformLocation(s.programID, "material.emissiveValue"),
			materialData.emissionColor.r, materialData.emissionColor.g, materialData.emissionColor.b); 
	}
	else
	{
		glUniform1i(glGetUniformLocation(s.programID, "material.emissiveEnabled"), 1);
	}

	if (!normalLoaded)
	{
		glUniform1i(glGetUniformLocation(s.programID, "material.normalEnabled"), 0);
	}
	else
	{
		glUniform1i(glGetUniformLocation(s.programID, "material.normalEnabled"), 1);
	}



	glUniform1f(glGetUniformLocation(s.programID, "material.shininess"), materialData.shininess);

	glUniform1f(glGetUniformLocation(s.programID, "material.emissiveForce"), materialData.emissionPower);

	// Load lights
	glUniform1i(glGetUniformLocation(s.programID, "pointLightCount"), lScene.pointLightCount);
	glUniform1i(glGetUniformLocation(s.programID, "dirLightActive"), (int)lScene.dirLightEnabled);

	glUniform2f(glGetUniformLocation(s.programID, "material.diffuseOffset"), 
		materialData.diffuseOffset.x, 
		materialData.diffuseOffset.y);

	glUniform2f(glGetUniformLocation(s.programID, "material.emissiveOffset"),
		materialData.emissiveOffset.x,
		materialData.emissiveOffset.y);

	glUniform3f(glGetUniformLocation(s.programID, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	for (int i = 0; i < lScene.pointLightCount; i++)
	{
		stringstream prefix;
		prefix << "pointLights[" << i << "]";
		
		std::string comp = prefix.str();	comp += ".position";
		glUniform3f(glGetUniformLocation(s.programID, comp.c_str()), 
			lScene.pointLights[i].position.x, 
			lScene.pointLights[i].position.y, 
			lScene.pointLights[i].position.z);
		comp = prefix.str();  comp += ".ambient";
		glUniform3f(glGetUniformLocation(s.programID, comp.c_str()), 
			lScene.pointLights[i].ambient.x, 
			lScene.pointLights[i].ambient.y,
			lScene.pointLights[i].ambient.z);
		comp = prefix.str(); comp += ".diffuse";
		glUniform3f(glGetUniformLocation(s.programID, comp.c_str()),
			lScene.pointLights[i].diffuse.x,
			lScene.pointLights[i].diffuse.y,
			lScene.pointLights[i].diffuse.z);
		comp = prefix.str(); comp += ".specular";
		glUniform3f(glGetUniformLocation(s.programID, comp.c_str()),
			lScene.pointLights[i].specular.x,
			lScene.pointLights[i].specular.y,
			lScene.pointLights[i].specular.z);
		comp = prefix.str(); comp += ".constant";
		glUniform1f(glGetUniformLocation(s.programID, comp.c_str()), lScene.pointLights[i].constant);
		comp = prefix.str(); comp += ".linear";
		glUniform1f(glGetUniformLocation(s.programID, comp.c_str()), lScene.pointLights[i].linear);
		comp = prefix.str(); comp += ".quadratic";
		glUniform1f(glGetUniformLocation(s.programID, comp.c_str()), lScene.pointLights[i].quadratic);
	}

	if (lScene.dirLightEnabled)
	{
		glUniform3f(glGetUniformLocation(s.programID, "dirLight.direction"),
			lScene.dirLight.direction.x,
			lScene.dirLight.direction.y,
			lScene.dirLight.direction.z);
		glUniform3f(glGetUniformLocation(s.programID, "dirLight.ambient"),
			lScene.dirLight.ambient.x,
			lScene.dirLight.ambient.y,
			lScene.dirLight.ambient.z);
		glUniform3f(glGetUniformLocation(s.programID, "dirLight.diffuse"),
			lScene.dirLight.diffuse.x,
			lScene.dirLight.diffuse.y,
			lScene.dirLight.diffuse.z);
		glUniform3f(glGetUniformLocation(s.programID, "dirLight.specular"),
			lScene.dirLight.specular.x,
			lScene.dirLight.specular.y,
			lScene.dirLight.specular.z);

	}


	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::Mesh()
{

}

void Mesh::setup()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, texCoords));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, bittangent));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}

Mesh::Mesh(vector<Vertex> v, vector<GLuint> i, vector<Texture> t, std::string name)
{
	this->name = name;
	build(v, i, t);
}


Mesh::~Mesh()
{
}

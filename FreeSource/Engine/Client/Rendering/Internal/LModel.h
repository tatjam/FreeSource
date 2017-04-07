#pragma once



#include <string>
#include <vector>
#include <map>

#include "../../AssetManager.h"
#include "Mesh.h"
#include "Shader.h"


#include "glm/glm.hpp"


#ifndef GRAPHICS_INCLUDED
#define GRAPHICS_INCLUDED
#define GLEW_STATIC
#include "GL/glew.h"
#define GLFW_INCLUDE_GLU
#include "GLFW/glfw3.h"

#endif






#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>


#include "../../../Util/loguru.hpp"

class AssetManager;

#define DEFAULT_ASSIMP_SETTINGS aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace


// The low level model implementation
// You should really use Model unless you want to 
// use the 3D engine directly without the Renderer, etc...
class LModel
{
public:

	AssetManager* assetManager;

	vector<Mesh> meshes;

	glm::mat4 transform;

	void draw(Shader s, LightScene lScene, glm::mat4 transform, glm::mat4 world, glm::mat4 persp, GLuint shadowMapID, GLuint sky, glm::vec3 cPos);
	void drawShadow(Shader s, glm::mat4 light, glm::mat4 transform);
	LModel(const GLchar* path, AssetManager* assetManager, unsigned int pflags = DEFAULT_ASSIMP_SETTINGS);
	~LModel();


private:

	std::string directory;
	void loadModel(std::string path, unsigned int pflags);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene, aiString name);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


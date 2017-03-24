#include "LModel.h"



LModel::LModel(const GLchar* path, unsigned int pFlags)
{
	loadModel(std::string(path), pFlags);
}

void LModel::drawShadow(Shader shader, glm::mat4 light, glm::mat4 transform)
{
	shader.use();
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].drawShadow(shader, light, transform);
	}
}

void LModel::draw(Shader shader, LightScene lScene, glm::mat4 transform, glm::mat4 world, glm::mat4 persp, GLuint shadowMapID)
{
	shader.use();
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader, lScene, transform, world, persp, shadowMapID);
	}
}

vector<Texture> LModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = AssetManager::getInstance().loadTexture(directory + "/" + str.C_Str());
		texture.type = typeName;
		textures.push_back(texture);

		std::cout << "Loaded new texture with id: " << texture.id << std::endl;
	}
	return textures;
}

void LModel::loadModel(string path, unsigned int pFlags)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, pFlags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "[LOADING MODEL ERROR] Assimp could not load model" << std::endl;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}

void LModel::processNode(aiNode* node, const aiScene* scene)
{
	// Process all meshes 
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene, node->mName));
	}
	// Keep this running for children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh LModel::processMesh(aiMesh* mesh, const aiScene* scene, aiString name)
{
	vector<Vertex> vvector;
	vector<GLuint> ivector;
	vector<Texture> tvector;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 vv;
		vv.x = mesh->mVertices[i].x;
		vv.y = mesh->mVertices[i].y;
		vv.z = mesh->mVertices[i].z;
		vertex.position = vv;

		vv.x = mesh->mNormals[i].x;
		vv.y = mesh->mNormals[i].y;
		vv.z = mesh->mNormals[i].z;
		vertex.normal = vv;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 tc;
			tc.x = mesh->mTextureCoords[0][i].x;
			tc.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = tc;
		}
		else
		{
			vertex.texCoords = glm::vec2(0, 0);
		}

		vvector.push_back(vertex);


		

	}


	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			ivector.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuses = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "diffuse");
		vector<Texture> speculars = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "specular");
		vector<Texture> emissives = loadMaterialTextures(material,
			aiTextureType_EMISSIVE, "emissive");

		Texture diffuse;
		Texture specular;
		Texture emissive;

		if (diffuses.size() > 0)
		{
			diffuse = diffuses[0];
		}
		if (speculars.size() > 0)
		{
			specular = speculars[0];
		}
		if (emissives.size() > 0)
		{
			emissive = emissives[0];
		}


		tvector.push_back(diffuse);
		tvector.push_back(specular);
		tvector.push_back(emissive);

	}

	return Mesh(vvector, ivector, tvector, name.C_Str());

}

LModel::~LModel()
{
}

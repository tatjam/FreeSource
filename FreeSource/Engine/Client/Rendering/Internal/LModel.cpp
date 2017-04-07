#include "LModel.h"



LModel::LModel(const GLchar* path,AssetManager* as, unsigned int pFlags)
{
	assetManager = as;
	loadModel(std::string(path), pFlags);
}

void LModel::drawShadow(Shader shader, glm::mat4 light, glm::mat4 transform)
{
	shader.use();
	for (int i = 0; i < (int)meshes.size(); i++)
	{
		meshes[i].drawShadow(shader, light, transform);
	}
}

void LModel::draw(Shader shader, LightScene lScene, glm::mat4 transform, glm::mat4 world, 
	glm::mat4 persp, GLuint shadowMapID, GLuint sky, glm::vec3 cPos)
{
	shader.use();
	for (int i = 0; i < (int)meshes.size(); i++)
	{
		meshes[i].draw(shader, lScene, transform, world, persp, shadowMapID, sky, cPos);
	}
}

vector<Texture> LModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = assetManager->loadTexture(directory + "/" + str.C_Str());
		texture.type = typeName;
		textures.push_back(texture);
	}
	return textures;
}

void LModel::loadModel(std::string path, unsigned int pFlags)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, pFlags);


	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_F(ERROR, "Error loading model! (Path: %s)", path.c_str());
		return;
	}

	aiApplyPostProcessing(scene, pFlags);

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

	Mesh m = Mesh();
	vector<Vertex> vvector;
	vector<GLuint> ivector;
	vector<Texture> tvector;

	if (mesh->HasTangentsAndBitangents() == false)
	{
		// Should not happen unless model has no UVs
		LOG_F(ERROR, "Could not generate tangents!");
	}

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
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

		if (mesh->mTangents == NULL)
		{
			vertex.tangent = glm::vec3();
			vertex.bittangent = glm::vec3();
		}
		else
		{

			vv.x = mesh->mTangents[i].x;
			vv.y = mesh->mTangents[i].y;
			vv.z = mesh->mTangents[i].z;
			vertex.tangent = vv;

			vv.x = mesh->mBitangents[i].x;
			vv.y = mesh->mBitangents[i].y;
			vv.z = mesh->mBitangents[i].z;
			vertex.bittangent = vv;
		}

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
		vector<Texture> normals = loadMaterialTextures(material,
			aiTextureType_HEIGHT, "normal");

		// NOTE THAT ASSIMP USES HEIGHT AS NORMAL!!!

		Texture diffuse;
		Texture specular;
		Texture emissive;
		Texture normal;

		if (diffuses.size() > 0)
		{
			diffuse = diffuses[0];
			tvector.push_back(diffuse);
		}
		if (speculars.size() > 0)
		{
			specular = speculars[0];
			tvector.push_back(specular);
		}
		if (emissives.size() > 0)
		{
			emissive = emissives[0];
			tvector.push_back(emissive);
		};
		if (normals.size() > 0)
		{
			normal = normals[0];
			tvector.push_back(normal);
		}


	}

	m = Mesh(vvector, ivector, tvector, name.C_Str());

	// Find material values
	if (mesh->mMaterialIndex >= 0)
	{
		LOG_F(INFO, "Loading material properties");

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D colorBuff;
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, colorBuff) != AI_SUCCESS)
		{
			LOG_F(ERROR, "Could not load Difusse property");
		}
		m.materialData.diffuseColor = glm::vec3(colorBuff.r, colorBuff.g, colorBuff.b);
		if (material->Get(AI_MATKEY_COLOR_SPECULAR, colorBuff) != AI_SUCCESS)
		{
			LOG_F(ERROR, "Could not load Specular property");
		}
		m.materialData.specularColor = glm::vec3(colorBuff.r, colorBuff.g, colorBuff.b);
		if (material->Get(AI_MATKEY_COLOR_EMISSIVE, colorBuff) != AI_SUCCESS)
		{
			LOG_F(ERROR, "Could not load Emissive property");
		}
		m.materialData.emissionColor = glm::vec3(colorBuff.r, colorBuff.g, colorBuff.b);

		if (material->Get(AI_MATKEY_COLOR_AMBIENT, colorBuff) != AI_SUCCESS)
		{
			LOG_F(ERROR, "Could not load Ambient property");
		}
		m.materialData.reflectiveColor = glm::vec3(colorBuff.r, colorBuff.g, colorBuff.b);


		float fBuff;
		if (material->Get(AI_MATKEY_SHININESS, fBuff) != AI_SUCCESS)
		{
			LOG_F(ERROR, "Could not load Shininess property");
		}


		m.materialData.shininess = fBuff;
		m.materialData.emissionPower = 1.0f;

	}

	return m;

}

LModel::~LModel()
{
}

#pragma once


#include "SOIL.h"

#include <map>

using std::map;

#include "Rendering/TextureData.h"
#include "Rendering/Model.h"

class LModel;


/*
#ifndef LOGURU_INCLUDED
#define LOGURU_INCLUDED
#include "../Util/loguru.hpp"
#endif
*/

class TextureData;
/*
struct Color8;
struct Color16;
struct Color32;
*/

class AssetManager
{
public:

	// For now only does textures

	map<std::string, GLuint> textures;
	map<std::string, TextureData*> texturesData;
	map<std::string, LModel*> models;
	map<std::string, Shader*> shaders;

	GLuint loadTexture(std::string path, int channels = SOIL_LOAD_RGB,
		bool storeData = true, bool force = false);
	GLuint loadTexture(std::string path, std::string name, int channels = SOIL_LOAD_RGB, 
		bool storeData = true, bool force = false);
	GLuint loadTexture(unsigned char* data, int data_size, std::string name, int channels = SOIL_LOAD_RGB, 
		bool storeData = true, bool force = false);

	// Reuploads texture into GL after being modified.
	// Faster and more limited version
	void updateTextureFast(std::string name);

	// Reuploads texture into GL after being modified.
	// Slower and less limited version
	void updateTextureSlow(std::string name);


	void loadShader(std::string vertexPath, std::string fragmentPath);
	void loadModel(std::string path, std::string name);

	LModel* getModel(std::string name);
	Shader* getShader(std::string name);
	TextureData* getTexture(std::string name);

	AssetManager()
	{

	}
	~AssetManager()
	{
		// Unload stuff (TODO)
	}
};


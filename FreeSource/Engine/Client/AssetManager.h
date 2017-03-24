#pragma once

#include "Rendering/Model.h"


#include "SOIL.h"

#include <map>

#define AssetManagerI AssetManager::getInstance()

using std::map;

class LModel;

#include "Rendering/TextureData.h"


class TextureData;
/*
struct Color8;
struct Color16;
struct Color32;
*/

class AssetManager
{
public:



	/*static std::string getColorString(Color color)
	{
		std::stringstream ss = std::stringstream();
		ss << "R{" + color.r << "}, G{" << color.g << "}, B{" << color.b << ", A{" << color.a << "}";
		return ss.str();
	}
	*/
	
	static AssetManager& getInstance()
	{
		static AssetManager    instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}

	// For now only does textures

	map<string, GLuint> textures;
	map<string, TextureData*> texturesData;
	map<string, LModel*> models;
	map<string, Shader*> shaders;

	GLuint loadTexture(string path, int channels = SOIL_LOAD_RGB, bool storeData = true);
	GLuint loadTexture(string path, string name, int channels = SOIL_LOAD_RGB, bool storeData = true);
	void loadShader(string vertexPath, string fragmentPath);
	void loadModel(string path, string name);

	LModel* getModel(string name);
	Shader* getShader(string name);
	TextureData* getTexture(string name);

private:
	AssetManager()
	{

	}
	~AssetManager()
	{

	}
	AssetManager(AssetManager const&);              // Don't Implement
	void operator=(AssetManager const&);			// Don't implement
};


#include "AssetManager.h"


GLuint AssetManager::loadTexture(string path, int channels, bool storeData)
{
	return loadTexture(path, path, channels, storeData);
}

TextureData* AssetManager::getTexture(string name)
{
	if (texturesData.count(name))
	{
		return texturesData[name];
	}
	return NULL;
}

GLuint AssetManager::loadTexture(string path, string name, int channels, bool storeData)
{
	if (textures.count(name))
	{
		std::cout << "TEXTURE ALREADY LOADED" << std::endl;
		return textures[name];
	}
	else
	{

		std::cout << "LOADING: " << path << " WITH NAME: " << name << std::endl;

		GLuint tId;
		glGenTextures(1, &tId);
		int width, height, channelsS;
		unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, &channelsS, channels);

		if (image == 0)
		{
			std::cout << "ERROR LOADING IMAGE: " << path << std::endl;
			return 0;
		}

		if (storeData)
		{
			TextureData* d = new TextureData();
			d->data = image;
			d->size = width * height;
			d->sizePerPixel = channelsS;
			d->width = width;
			d->height = height;
			texturesData[name] = d;
		}

		glBindTexture(GL_TEXTURE_2D, tId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		if (!storeData)
		{
			SOIL_free_image_data(image);
		}
		textures[name] = tId;


		std::cout << "DONE LOADED TO " << tId << std::endl;

		return textures[name];
	}
}

void AssetManager::loadModel(string path, string name)
{
	if (models.count(name))
	{
		models[std::string(name + ".REPLACED")] = models[name];
	}

	LModel* n = new LModel(path.c_str());
	models[name] = n;
}

LModel* AssetManager::getModel(string name)
{
	if (models.count(name))
	{
		return models[name];
	}
	else
	{
		return NULL;
	}
}

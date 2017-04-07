#include "AssetManager.h"


TextureData* AssetManager::getTexture(std::string name)
{
	if (texturesData.count(name))
	{
		return texturesData[name];
	}
	return NULL;
}

void AssetManager::updateTextureFast(std::string name)
{
	if (texturesData.count(name))
	{
		glBindTexture(GL_TEXTURE_2D, texturesData[name]->glId);
		/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			texturesData[name]->width, texturesData[name]->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, texturesData[name]->data);*/

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texturesData[name]->width, 
			texturesData[name]->height, GL_SRGB, GL_UNSIGNED_BYTE, texturesData[name]->data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	

}

void AssetManager::updateTextureSlow(std::string name)
{
	if (texturesData.count(name))
	{
		glBindTexture(GL_TEXTURE_2D, texturesData[name]->glId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB,
		texturesData[name]->width, texturesData[name]->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, texturesData[name]->data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

GLuint AssetManager::loadTexture(std::string path, int channels, bool storeData, bool force)
{
	return loadTexture(path, path, channels, storeData, force);
}

GLuint AssetManager::loadTexture(unsigned char* data, int data_size, std::string name, int channels, bool storeData, bool force)
{
	LOG_F(INFO, "Loading texture from memory with name '%s'", name.c_str());

	if (textures.count(name) && !force)
	{
		LOG_F(INFO, "Texture is already loaded!");
		return textures[name];
	}
	else
	{
		GLuint tId;
		glGenTextures(1, &tId);
		int width, height, channelsS;
		unsigned char* image = SOIL_load_image_from_memory(data, data_size, &width, &height, &channelsS, channels);

		if (image == 0)
		{
			LOG_F(ERROR, "Could not load texture!");
			return 0;
		}


		glBindTexture(GL_TEXTURE_2D, tId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (storeData)
		{
			TextureData* d = new TextureData();
			d->data = image;
			d->size = width * height;
			d->sizePerPixel = channels;
			d->width = width;
			d->height = height;
			d->glId = tId;
			texturesData[name] = d;
		}


		if (!storeData)
		{
			SOIL_free_image_data(image);
		}
		textures[name] = tId;


		LOG_F(INFO, "Loaded texture from memory! (ID: %i)", tId);

		return textures[name];
	}
}

GLuint AssetManager::loadTexture(std::string path, std::string name, int channels, bool storeData, bool force)
{
	LOG_F(INFO, "Loading texture from path:'%s' with name '%s'", path.c_str(), name.c_str());

	if (textures.count(name) && !force)
	{
		LOG_F(INFO, "Texture is already loaded!");
		return textures[name];
	}
	else
	{

		GLuint tId;
		glGenTextures(1, &tId);
		int width, height, channelsS;
		unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, &channelsS, channels);

		if (image == 0)
		{
			LOG_F(ERROR, "Could not load texture!");
			return 0;
		}


		glBindTexture(GL_TEXTURE_2D, tId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);


		if (storeData)
		{
			TextureData* d = new TextureData();
			d->data = image;
			d->size = width * height;
			d->sizePerPixel = channels;
			d->width = width;
			d->height = height;
			d->glId = tId;
			texturesData[name] = d;
		}


		if (!storeData)
		{
			SOIL_free_image_data(image);
		}
		textures[name] = tId;


		LOG_F(INFO, "Loaded texture! (ID: %i)", tId);

		return textures[name];
	}
}

TextureData* AssetManager::loadTextureData(std::string path, std::string name, int channels, bool force)
{

	LOG_F(INFO, "Loading texture from path:'%s' with name '%s' (DATA ONLY)", path.c_str(), name.c_str());
	int width, height, channelsS;
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, &channelsS, channels);

	if (image == 0)
	{
		LOG_F(ERROR, "Could not load texture!");
		return NULL;
	}

	TextureData* d = new TextureData();
	d->data = image;
	d->size = width * height;
	d->sizePerPixel = channels;
	d->width = width;
	d->height = height;
	d->glId = -1;
	texturesData[name] = d;

	return texturesData[name];
}

GLuint AssetManager::createCubemap(TextureData** data, std::string name, bool correct, bool force)
{
	LOG_F(INFO, "Creating cubemap with name: %s", name.c_str());
	if (cubemaps.count(name) && !force)
	{
		return cubemaps[name];
	}
	else
	{
		GLuint tId;
		glGenTextures(1, &tId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tId);

		for (GLuint i = 0; i < 6; i++)
		{
			int face = 0;
			if (correct)
			{
				if (i == 0)
				{
					face = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
				}
				else if (i == 1)
				{
					face = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
				}
				else if (i == 2)
				{
					face = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
				}
				else if (i == 3)
				{
					face = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
				}
				else if (i == 4)
				{
					face = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
				}
				else
				{
					face = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
				}
			}
			else
			{ 
				face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			}
			glTexImage2D(face, 0, GL_SRGB, data[i]->width, data[i]->height, 
				0, GL_RGB, GL_UNSIGNED_BYTE, data[i]->data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		cubemaps[name] = tId;
		return tId;
	}
}

void AssetManager::loadModel(std::string path, std::string name)
{
	if (models.count(name))
	{
		models[std::string(name + ".REPLACED")] = models[name];
	}

	LModel* n = new LModel(path.c_str(), this, DEFAULT_ASSIMP_SETTINGS);
	models[name] = n;
}

LModel* AssetManager::getModel(std::string name)
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

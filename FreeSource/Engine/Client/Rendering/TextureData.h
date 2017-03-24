#pragma once
#include "../AssetManager.h"

class AssetManager;

struct Color8
{
	uint8_t r, g, b, a;
};

struct Color16
{
	uint16_t r, g, b, a;
};

struct Color32
{
	uint32_t r, g, b, a;
};

#define Color Color8

class TextureData
{

	friend class AssetManager;

public:

	int width, height;
	Color* colors;
	int colorsSize;

	// Get access to low level data
	unsigned char* getData()
	{
		return data;
	}

	// Will create the color array
	// Not really required unless you want
	// to do a lot of operations over
	// the texture
	void createColorArray();

	// Gets a pixel
	Color getPixel(int x, int y);

	// Sets a pixel (it does not update GL texture!)
	void setPixel(int x, int y, Color c);

private:
	unsigned char* data;

	int size;
	int sizePerPixel;


	Color formColor(int i);

	TextureData();
	~TextureData();
};

#pragma once
#include "../AssetManager.h"

#include "../../Util/tinyterm.h"

class AssetManager;

struct Color8
{
	uint8_t r, g, b, a;
	Color8(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		this->r = r; this->g = g; this->b = b; this->a = a;
	}
	Color8(uint8_t r, uint8_t g, uint8_t b)
	{
		this->r = r; this->g = g; this->b = b; this->a = 0;
	}
	Color8() {}
};

struct Color16
{
	uint16_t r, g, b, a;
	Color16(uint16_t r, uint16_t g, uint16_t b, uint16_t a)
	{
		this->r = r; this->g = g; this->b = b; this->a = a;
	}
	Color16(uint16_t r, uint16_t g, uint16_t b)
	{
		this->r = r; this->g = g; this->b = b; this->a = 0;
	}
	Color16() {}
};

struct Color32
{
	uint32_t r, g, b, a;
	Color32(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		this->r = r; this->g = g; this->b = b; this->a = a;
	}
	Color32(uint32_t r, uint32_t g, uint32_t b)
	{
		this->r = r; this->g = g; this->b = b; this->a = 0;
	}
	Color32() {}
};

#define Color Color8

class TextureData
{

	friend class AssetManager;

public:

	int width, height;
	int glId;
	Color* colors;
	int colorsSize;

	unsigned char* data;

	int size;
	int sizePerPixel;

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



	Color formColor(int i);

	TextureData();
	~TextureData();
};

#include "TextureData.h"

Color TextureData::getPixel(int x, int y)
{
	if (colors)
	{
		Color out;
		if (x < width && y < height && x > 0 && y > 0)
		{
			out = colors[width*x + y];
		}
		return out;
	}
	else
	{
		Color out;
		if (x < width && y < height && x > 0 && y > 0)
		{
			out = formColor(width*x + y);
		}
		return out;
	}
}

void TextureData::setPixel(int x, int y, Color c)
{
	if (x < width && y < height && x > 0 && y > 0)
	{
		int i = width * x + y;
		i *= sizePerPixel;
		if (i < size * sizePerPixel)
		{
			data[i] = c.r;
			data[i + 1] = c.g;
			data[i + 2] = c.b;
		}
	}
	
}

Color TextureData::formColor(int i)
{
	
	Color out;
	i *= sizePerPixel;

	if (sizePerPixel == 1)
	{
		out.r = data[i];
	} 
	else if (sizePerPixel == 2)
	{
		out.r = data[i];
		out.g = data[i + 1];
	} 
	else if (sizePerPixel == 3)
	{
		out.r = data[i];
		out.g = data[i + 1];
		out.b = data[i + 2];
	} 
	else
	{
		out.r = data[i];
		out.g = data[i + 1];
		out.b = data[i + 2];
		out.a = data[i + 3];
		//Ignore other possible channels (Could be improved!)
	}
	return out;
	
}

void TextureData::createColorArray()
{
	
	colors = (Color*)malloc(size * sizeof(Color));
	for (int i = 0; i < size; i++)
	{
		colors[i] = formColor(i);
	}
	
}

TextureData::TextureData()
{
}


TextureData::~TextureData()
{
	if (colorsSize != 0)
	{
		free(colors);
	}
}

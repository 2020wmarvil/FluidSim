#pragma once

#include <iostream>

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct Texture {
    int width;
    int height;
    Color *data;

    Texture(int width, int height)
    {
        this->width = width;
        this->height = height;
        this->data = (Color*)calloc(width * height, sizeof(Color));
    }
};

class FluidSim2D {
public:
	FluidSim2D(int width, int height)
	{
		this->width = width;
		this->height = height;
		texture_data = (Color*)malloc(width * height * sizeof(Color));
	}

	void Step(float dt)
	{
		for (int index = 0; index < width * height; index++)
		{
			texture_data[index] = { (unsigned char)(index / (0.f + width * height) * 255), 0, 0 };
		}
	}

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	Color* GetTextureData()
	{
		return texture_data;
	}

private:
	int width;
	int height;
	Color* texture_data;
};
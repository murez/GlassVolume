#pragma once
#define GLEW_STATIC

#include <iostream>
#include <tiffio.h>

#include <GL/glew.h>

class TIFFTexture
{
	uint8_t *data;
	uint8_t *gradient;
	GLuint texture;
public:
	TIFFTexture(std::string filepath);
	int width, height, depth, time;
	~TIFFTexture();
};


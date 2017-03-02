#pragma once
#include "GL/gl3w.h"
#include <string>
#include "FreeImage.h"

class Texture
{
	FIBITMAP *img;
	GLuint id;
public:
	Texture(std::string path);
	~Texture();
	void load();
	int getWidth();
	int getHeight();
};


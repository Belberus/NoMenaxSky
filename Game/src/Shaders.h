#pragma once
#include "GL/gl3w.h"
#include <string>

class Shaders
{
	std::string vertexShader;
	std::string fragmentShader;
	GLuint program;

public:
	Shaders();
	~Shaders();
	GLuint getProgram();
	void useProgram();
};


#pragma once
#include "GL/glew.h"
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


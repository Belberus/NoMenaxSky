#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Shaders.h"

class Actor
{
	glm::vec3 position;
	glm::vec2 size;
	Texture &texture;
	Shaders &shaders;
	GLuint vao;
	GLuint ebo;
	GLuint buf;
	GLfloat quad[16] = {
		// Vertices		// Texture coordinates
		0.0f, 0.0f,		0.0f, 0.0f,
		1.0f, 0.0f,		1.0f, 0.0f,
		1.0f, 1.0f,		1.0f, 1.0f,
		0.0f, 1.0f,		0.0f, 1.0f
	};
	GLuint indices[6] = {
		0, 1, 2,
		0, 3, 2
	};

public:
	Actor(glm::vec3 position, glm::vec2 size, Texture &texture, Shaders &shaders);
	void draw();
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();
	~Actor();
};


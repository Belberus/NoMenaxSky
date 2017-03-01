#include "Actor.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Actor::Actor(glm::vec3 position, glm::vec2 size, Texture &texture, Shaders &shaders) : 
	position(position), size(size), texture(texture), shaders(shaders)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &buf);
}

void Actor::draw()
{
	texture.load();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	shaders.useProgram();

	glUniformMatrix4fv(glGetUniformLocation(shaders.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaders.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Actor::setPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 Actor::getPosition()
{
	return position;
}


Actor::~Actor()
{
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &buf);
	glDeleteVertexArrays(1, &vao);
}

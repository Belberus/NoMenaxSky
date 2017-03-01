#define GLEW_STATIC

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "Texture.h"
#include "Shaders.h"
#include "FreeImage.h"
#include <iostream>
#include <memory>
#include "Actor.h"
#include "Scene.h"

// MAIN SOLO PARA TEST

std::shared_ptr<Actor> aux;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		glm::vec3 oldPosition = aux->getPosition();
		aux->setPosition(glm::vec3(oldPosition.x, oldPosition.y + 10, oldPosition.z));
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		glm::vec3 oldPosition = aux->getPosition();
		aux->setPosition(glm::vec3(oldPosition.x, oldPosition.y - 10, oldPosition.z));
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		glm::vec3 oldPosition = aux->getPosition();
		aux->setPosition(glm::vec3(oldPosition.x - 10, oldPosition.y, oldPosition.z));
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		glm::vec3 oldPosition = aux->getPosition();
		aux->setPosition(glm::vec3(oldPosition.x + 10, oldPosition.y, oldPosition.z));
	}

}

int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	GLFWwindow* window = glfwCreateWindow(960, 540, "No Mena's Sky", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	glViewport(0, 0, 960, 540);
	glClearColor(1, 0, 0, 0);
	glfwSetKeyCallback(window, key_callback);
	Shaders shaders;
	Texture texture1("img.png");
	Texture texture2("img2.png");
	std::shared_ptr<Actor> actor1(new Actor(glm::vec3(0, 0, 0), glm::vec2(960, 540), texture1, shaders));
	std::shared_ptr<Actor> actor2( new Actor(glm::vec3(480, 270, 1), glm::vec2(100, 100), texture2, shaders));
	aux = actor2;
	Scene scene;
	scene.addActor(*actor1.get());
	scene.addActor(*actor2.get());
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		scene.draw();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

#include "engine/core/scene_manager.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>

engine::core::SceneManager::SceneManager(GLFWwindow* window)
    : window_(window) {}

void engine::core::SceneManager::Play(Scene& scene) {
  entityx::TimeDelta dt = 0.0f;
  while (!glfwWindowShouldClose(window_)) {
    entityx::TimeDelta t1 = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.Update(dt);
    glfwSwapBuffers(window_);
    glfwPollEvents();
    entityx::TimeDelta t2 = glfwGetTime();
    dt = t2 - t1;
  }
}

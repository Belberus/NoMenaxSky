#include <GL/gl3w.h>
#include "Window.h"
#include <iostream>

Window::Window(std::string title, int width, int height)
    : title(title), width(width), height(height) {
  init();
}

void Window::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

GLFWwindow *Window::getGLFWwindow() { return window; }

void Window::pollEvents() { glfwPollEvents(); }

void Window::swapBuffers() { glfwSwapBuffers(window); }

void resize(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void Window::update(){
  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool Window::init() {
  if (!glfwInit()) {
    std::cout << "GLFW init error" << std::endl;
    return false;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cout << "Failed to create GLFW window" << std::endl;
    return false;
  }
  glfwMakeContextCurrent(window);
  if (gl3wInit()) {
    glfwTerminate();
    std::cout << "GL3W init error" << std::endl;
    return false;
  }
  glfwSetWindowSizeCallback(window, resize);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
  glfwSwapInterval(1);
  // glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  return true;
}

bool Window::isClosed() { return glfwWindowShouldClose(window); }

int Window::getWidth() { return width; }

int Window::getHeight() { return height; }

Window::~Window() { glfwTerminate(); }

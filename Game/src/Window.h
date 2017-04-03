#pragma once
#include <GLFW/glfw3.h>
#include <string>
class Window {
  std::string title;
  int height;
  int width;
  GLFWwindow *window;

 public:
  Window(std::string title, int width, int height);
  ~Window();
  void clear();
  void swapBuffers();
  void pollEvents();
  void update();
  bool isClosed();
  int getWidth();
  int getHeight();
  bool isKeyPressed(int key);
  GLFWwindow *getGLFWwindow();

 private:
  bool init();
};

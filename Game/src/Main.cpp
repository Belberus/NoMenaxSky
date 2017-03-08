#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "SceneFactory.h"
#include "Shaders.h"
#include "Window.h"

int main() {
  Window window("No Mena's Sky", 960, 540);
  Shaders shaders;
  Scene *scene1 = SceneFactory::get()->getScene("scene1");
  while (!window.isClosed()) {
    window.clear();
    scene1->update(window, shaders);
    window.update();
  }
  SceneFactory::get()->clear();
}

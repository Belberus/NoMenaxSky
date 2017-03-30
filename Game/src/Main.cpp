#include <GL/gl3w.h>
#include "Room.h"
#include <GLFW/glfw3.h>

int main() {
  Window window("No Menax Sky", 960, 540);
  Shaders shaders;
  Room room(window, shaders);
  double t1_f = glfwGetTime();
  int frames = 0;
  entityx::TimeDelta dt = 0;
  while (!window.isClosed()) {
    // TODO: mover calculo dt a la clase Window
    entityx::TimeDelta t1 = glfwGetTime();
    window.clear();
    window.pollEvents();
    room.update(dt);
    window.swapBuffers();
    entityx::TimeDelta t2 = glfwGetTime();
    dt = t2 - t1;
  }
}

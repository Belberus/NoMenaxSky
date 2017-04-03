#include "Game.h"
#include <GLFW/glfw3.h>

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

int main() {
  Window window("No Menax Sky", 960, 540);
  Shaders shaders;
  Game game(window, shaders);
  double t1_f = glfwGetTime();
  entityx::TimeDelta dt = 0;
  while (!window.isClosed()) {
    // TODO: mover calculo dt a la clase Window
    entityx::TimeDelta t1 = glfwGetTime();
    window.clear();
    game.update(dt);
    window.update();
    entityx::TimeDelta t2 = glfwGetTime();
    dt = t2 - t1;
  }
}

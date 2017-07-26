#include "engine/core/input_manager.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "engine/core/engine.h"
#include "engine/core/event_manager.h"
#include "engine/events/key_pressed.h"
#include "engine/events/key_released.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mods) {
  if (action == GLFW_PRESS) {
    engine::core::Engine::GetInstance()
        .Get<engine::core::EventManager>()
        .Emit<engine::events::KeyPressed>(key);
  } else if (action == GLFW_RELEASE) {
    engine::core::Engine::GetInstance()
        .Get<engine::core::EventManager>()
        .Emit<engine::events::KeyReleased>(key);
  }
}

engine::core::InputManager::InputManager(GLFWwindow* window) {
  glfwSetKeyCallback(window, KeyCallback);
}

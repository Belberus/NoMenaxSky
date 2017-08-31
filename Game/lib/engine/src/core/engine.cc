#include "engine/core/engine.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <string>

#include "engine/core/audio_manager.h"
#include "engine/core/event_manager.h"
#include "engine/core/input_manager.h"
#include "engine/core/resource_manager.h"
#include "engine/core/scene_manager.h"

engine::core::Engine::Engine() {
  if (!glfwInit()) {
    std::cout << "GLFW init error" << std::endl;
    exit(-1);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_RESIZABLE,
                 properties_.resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfw_window_ = glfwCreateWindow(
      properties_.width, properties_.height, properties_.title.c_str(),
      properties_.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
  if (!glfw_window_) {
    glfwTerminate();
    std::cout << "Failed to create GLFW window" << std::endl;
    exit(-1);
  }
  glfwMakeContextCurrent(glfw_window_);
  if (gl3wInit()) {
    glfwTerminate();
    std::cout << "GL3W init error" << std::endl;
    exit(-1);
  }
  glfwSwapInterval(properties_.vsync);
  glClearColor(47.0f / 255.0f, 40.0f / 255.0f, 58.0f / 255.0f, 255.0f / 255.0f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  AddServiceProvider<AudioManager>();
  AddServiceProvider<EventManager>();
  AddServiceProvider<InputManager>(glfw_window_);
  AddServiceProvider<SceneManager>(glfw_window_);
  AddServiceProvider<ResourceManager>();
}

void engine::core::Engine::Init(const Properties& properties) {
  properties_ = properties;
  if (!engine_) {
    engine_ = std::unique_ptr<Engine>(new Engine());
  }
}

engine::core::Engine& engine::core::Engine::GetInstance() { return *engine_; }

void engine::core::Engine::EnableDepthTest(DepthTest depth_test) {
  switch (depth_test) {
    case DepthTest::kLess:
      glDepthFunc(GL_LESS);
      break;
    case DepthTest::kAlways:
      glDepthFunc(GL_ALWAYS);
      break;
  }
  glEnable(GL_DEPTH_TEST);
}

engine::core::Engine::~Engine() {
  glfwDestroyWindow(glfw_window_);
  glfwTerminate();
}

engine::core::Engine::Properties engine::core::Engine::properties_ = {
    800, 600, "Default", false, false};

std::unique_ptr<engine::core::Engine> engine::core::Engine::engine_ = nullptr;

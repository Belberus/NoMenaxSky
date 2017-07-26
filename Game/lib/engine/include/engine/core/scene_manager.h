#ifndef ENGINE_CORE_SCENE_MANAGER_H_
#define ENGINE_CORE_SCENE_MANAGER_H_

#include <memory>

#include <entityx/entityx.h>

#include "engine/core/scene.h"

struct GLFWwindow;
namespace engine {
namespace core {
class SceneManager {
 public:
  SceneManager(GLFWwindow *window);
  void Play(Scene &scene);

 private:
  GLFWwindow *window_;
};
}  // namespace core
}  // namespace engine
#endif  // ENGINE_CORE_SCENE_MANAGER_H_

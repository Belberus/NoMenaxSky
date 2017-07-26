#ifndef ENGINE_CORE_INPUT_MANAGER_H_
#define ENGINE_CORE_INPUT_MANAGER_H_

struct GLFWwindow;
namespace engine {
namespace core {
class InputManager {
 public:
  InputManager(GLFWwindow *window);
};
}  // namespace core
}  // namespace engine
#endif  // ENGINE_CORE_INPUT_MANAGER_H_

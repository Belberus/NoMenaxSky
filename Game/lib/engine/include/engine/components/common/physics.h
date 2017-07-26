#ifndef ENGINE_COMPONENTS_COMMON_PHYSICS_H_
#define ENGINE_COMPONENTS_COMMON_PHYSICS_H_

#include <glm/glm.hpp>

namespace engine {
namespace components {
namespace common {
struct Physics {
  glm::vec3 velocity;
  Physics(glm::vec3 velocity) : velocity(velocity) {}
};
}  // namespace common
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_COMMON_PHYSICS_H_

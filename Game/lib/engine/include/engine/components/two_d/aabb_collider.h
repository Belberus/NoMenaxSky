#ifndef ENGINE_COMPONENTS_TWO_D_AABB_COLLIDER_H_
#define ENGINE_COMPONENTS_TWO_D_AABB_COLLIDER_H_
#include <glm/glm.hpp>

namespace engine {
namespace components {
namespace two_d {
struct AABBCollider {
  glm::vec2 center;
  glm::vec2 half_size;
  bool trigger;
  AABBCollider(glm::vec2 position, glm::vec2 size, bool trigger = false)
      : center(position), half_size(size), trigger(trigger) {}
};
}  // namespace two_d
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_TWO_D_AABB_COLLIDER_H_

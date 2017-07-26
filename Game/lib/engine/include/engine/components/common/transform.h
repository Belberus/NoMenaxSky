#ifndef ENGINE_COMPONENTS_COMMON_TRANSFORM_H_
#define ENGINE_COMPONENTS_COMMON_TRANSFORM_H_

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace engine {
namespace components {
namespace common {
class Transform {
 public:
  Transform(glm::vec3 position, Transform *parent = nullptr,
            glm::vec3 scale = glm::vec3(1.0f),
            glm::quat orientation = glm::quat());
  glm::mat4 GetWorldMatrix() const;
  glm::mat4 GetLocalMatrix() const;

  glm::vec3 GetWorldPosition() const;
  glm::vec3 GetWorldScale() const;
  glm::quat GetWorldOrientation() const;

  glm::vec3 GetLocalPosition() const;
  glm::vec3 GetLocalScale() const;
  glm::quat GetLocalOrientation() const;

  void SetLocalPosition(const glm::vec3 &pos);
  void SetLocalScale(const glm::vec3 &scale);
  void SetLocalOrientation(const glm::quat &orientation);

 private:
  Transform *parent_;
  glm::vec3 local_position_;
  glm::vec3 local_scale_;
  glm::quat local_orientation_;
};
}  // namespace common
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_COMMON_TRANSFORM_H_

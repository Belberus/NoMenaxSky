#ifndef ENGINE_COMPONENTS_COMMON_CAMERA_H_
#define ENGINE_COMPONENTS_COMMON_CAMERA_H_

#include <memory>
#include <limits>

#include <glm/glm.hpp>

#include "engine/components/common/transform.h"

namespace engine {
namespace components {
namespace common {
class Camera {
 public:
  enum class CameraType { kOrtho, kPerspective };
  // Orthographic camera constructor
  Camera(float width, float height,
         float nearPlane = (std::numeric_limits<float>::max)(),
         float farPlane = (std::numeric_limits<float>::min)());
  glm::mat4 GetViewMatrix(
      const engine::components::common::Transform &transform);
  glm::mat4 GetProjectionMatrix();

 private:
  CameraType camera_type_;

  // ortho
  float width_;
  float height_;
  float near_plane_;
  float far_plane_;
};
}  // namespace common
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_COMMON_CAMERA_H_

#ifndef ENGINE_COMPONENTS_COMMON_CAMERA_H_
#define ENGINE_COMPONENTS_COMMON_CAMERA_H_

#include <memory>

#include <glm/glm.hpp>

#include "engine/components/common/transform.h"

namespace engine {
namespace components {
namespace common {
class Camera {
 public:
  enum class CameraType { kOrtho, kPerspective };
  /// Constructs an orthographic camera.
  /// @param widht
  /// @param height
  /// @param near_plane
  /// @param far_plane
  Camera(float width, float height, float near_plane, float far_plane);

  /// Construct a perspective camera.
  /// @fov field of view of the camera
  Camera(float fov, float width, float height, float near_plane,
         float far_plane);

  /// Returns the view matrix of this camera.
  /// @param transform transform of the camera.
  /// @return view matrix that transforms from world space to view space.
  glm::mat4 GetViewMatrix(
      const engine::components::common::Transform &transform);

  /// Returns the projections matrix of this camera.
  /// @return projection matrix that transfroms from view space to clip space.
  glm::mat4 GetProjectionMatrix();

 private:
  CameraType camera_type_;

  float width_;
  float height_;
  float near_plane_;
  float far_plane_;
  float fov_;
};
}  // namespace common
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_COMMON_CAMERA_H_

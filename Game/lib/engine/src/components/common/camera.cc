#include "engine/components/common/camera.h"

#include <glm/gtc/matrix_transform.hpp>

engine::components::common::Camera::Camera(float width, float height,
                                           float near_plane, float far_plane)
    : camera_type_(CameraType::kOrtho),
      width_(width),
      height_(height),
      near_plane_(near_plane),
      far_plane_(far_plane) {}

glm::mat4 engine::components::common::Camera::GetViewMatrix(
    const engine::components::common::Transform &transform) {
  if (camera_type_ == CameraType::kOrtho) {
    glm::mat4 view_matrix = glm::mat4_cast(transform.GetWorldOrientation());
    view_matrix =
        glm::translate(view_matrix, -1.0f * transform.GetWorldPosition());
    return view_matrix;
  }
}

glm::mat4 engine::components::common::Camera::GetProjectionMatrix() {
  if (camera_type_ == CameraType::kOrtho) {
    return glm::ortho(-width_ / 2.0f, width_ / 2.0f, -height_ / 2.0f,
                      height_ / 2.0f, near_plane_, far_plane_);
  } else {
    return glm::mat4();
  }
}

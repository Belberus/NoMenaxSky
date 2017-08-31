#include "engine/components/common/camera.h"

#include <glm/gtc/matrix_transform.hpp>

engine::components::common::Camera::Camera(float width, float height,
                                           float near_plane, float far_plane)
    : camera_type_(CameraType::kOrtho),
      width_(width),
      height_(height),
      near_plane_(near_plane),
      far_plane_(far_plane) {}

engine::components::common::Camera::Camera(float fov, float width, float height,
                                           float near_plane, float far_plane)
    : camera_type_(CameraType::kPerspective),
      fov_(fov),
      width_(width),
      height_(height),
      near_plane_(near_plane),
      far_plane_(far_plane) {}

glm::mat4 engine::components::common::Camera::GetViewMatrix(
    const engine::components::common::Transform &transform) {
  return glm::inverse(transform.GetWorldMatrix());
}

glm::mat4 engine::components::common::Camera::GetProjectionMatrix() {
  if (camera_type_ == CameraType::kOrtho) {
    return glm::ortho(-width_ / 2.0f, width_ / 2.0f, -height_ / 2.0f,
                      height_ / 2.0f, near_plane_, far_plane_);
  } else {
    return glm::perspective(fov_, width_ / height_, near_plane_, far_plane_);
  }
}

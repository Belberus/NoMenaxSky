#include "engine/components/common/transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

engine::components::common::Transform::Transform(glm::vec3 position,
                                                 Transform* parent,
                                                 glm::vec3 scale,
                                                 glm::quat orientation)
    : parent_(parent),
      local_position_(position),
      local_scale_(scale),
      local_orientation_(orientation) {}

glm::mat4 engine::components::common::Transform::GetWorldMatrix() const {
  auto world_matrix = GetLocalMatrix();
  auto aux = parent_;
  while (aux != nullptr) {
    world_matrix *= aux->GetLocalMatrix();
    aux = aux->parent_;
  }
  return world_matrix;
}

glm::mat4 engine::components::common::Transform::GetLocalMatrix() const {
  return glm::translate(glm::mat4(1.0f), local_position_) *
         glm::mat4_cast(local_orientation_) *
         glm::scale(glm::mat4(1.0f), local_scale_);
}

glm::vec3 engine::components::common::Transform::GetWorldPosition() const {
  auto world_position = local_position_;
  auto aux = parent_;
  while (aux != nullptr) {
    world_position += parent_->GetLocalPosition();
    aux = aux->parent_;
  }
  return world_position;
}

glm::vec3 engine::components::common::Transform::GetWorldScale() const {
  auto world_scale = local_scale_;
  auto aux = parent_;
  while (aux != nullptr) {
    auto parentScale = aux->GetLocalScale();
    world_scale.x *= parentScale.x;
    world_scale.y *= parentScale.y;
    world_scale.z *= world_scale.z;
    aux = aux->parent_;
  }
  return world_scale;
}

glm::quat engine::components::common::Transform::GetWorldOrientation() const {
  auto world_orientation = local_orientation_;
  auto aux = parent_;
  while (aux != nullptr) {
    world_orientation *= parent_->GetLocalOrientation();
    aux = aux->parent_;
  }
  return world_orientation;
}

glm::vec3 engine::components::common::Transform::GetLocalPosition() const {
  return local_position_;
}

glm::vec3 engine::components::common::Transform::GetLocalScale() const {
  return local_scale_;
}

glm::quat engine::components::common::Transform::GetLocalOrientation() const {
  return local_orientation_;
}

void engine::components::common::Transform::SetLocalPosition(
    const glm::vec3& position) {
  this->local_position_ = position;
}

void engine::components::common::Transform::SetLocalScale(
    const glm::vec3& scale) {
  this->local_scale_ = scale;
}

void engine::components::common::Transform::SetLocalOrientation(
    const glm::quat& orientation) {
  this->local_orientation_ = orientation;
}

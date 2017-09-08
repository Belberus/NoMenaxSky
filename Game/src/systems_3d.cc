#include "systems_3d.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <glm/glm.hpp>

#include "components.h"

const float CameraFollowPlayerSystem::kCameraSpeed = 4.0f;

void CameraFollowPlayerSystem::update(entityx::EntityManager &es,
                                      entityx::EventManager &events,
                                      entityx::TimeDelta dt) {
  glm::vec3 player_pos;
  es.each<engine::components::common::Transform, Player>(
      [&](entityx::Entity entity,
          engine::components::common::Transform &transform,
          Player &player) { player_pos = transform.GetWorldPosition(); });
  es.each<engine::components::common::Transform,
          engine::components::common::Camera>(
      [&](entityx::Entity entity,
          engine::components::common::Transform &transform,
          engine::components::common::Camera &camera) {
        auto old_camera_pos = transform.GetLocalPosition();
        auto new_camera_pos =
            glm::mix(old_camera_pos, player_pos, dt * kCameraSpeed);
        new_camera_pos.z = old_camera_pos.z;
        transform.SetLocalPosition(new_camera_pos);
      });
}

void RotatePlayerSystem::update(entityx::EntityManager &es,
                                entityx::EventManager &events,
                                entityx::TimeDelta dt) {
  es.each<engine::components::common::Transform, Player>(
      [&](entityx::Entity entity,
          engine::components::common::Transform &transform, Player &player) {
        glm::quat rotation;
        switch (player.orientation) {
          case Player::Orientation::DOWN:
            break;
          case Player::Orientation::UP:
            break;
          case Player::Orientation::LEFT:
            break;
          case Player::Orientation::RIGHT:
            break;
        }
      });
}

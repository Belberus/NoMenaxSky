#include "systems_3d.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>

#include "components.h"

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
        auto camera_pos = transform.GetLocalPosition();
        camera_pos.x = player_pos.x;
        camera_pos.y = player_pos.y;
        transform.SetLocalPosition(camera_pos);
      });
}
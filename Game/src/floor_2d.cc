#include "floor_2d.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/systems/two_d/collider_renderer.h>
#include <engine/systems/two_d/color_animator.h>
#include <engine/systems/two_d/physics.h>
#include <engine/systems/two_d/sprite_animator.h>
#include <engine/systems/two_d/sprite_renderer.h>
#include <engine/systems/two_d/tilemap_renderer.h>

#include "components.h"
#include "floor.h"
#include "systems.h"

using namespace engine::systems::two_d;
using namespace engine::events;
using namespace engine::components::two_d;
using namespace engine::components::common;

Floor2D::Floor2D() {
  systems.add<PlayerInputSystem>();
  systems.add<GhostIaSystem>();
  systems.add<GhostAnimationSystem>();
  systems.add<TurretIaSystem>();
  systems.add<TrapIaSystem>();
  systems.add<TurretWalkingSystem>();
  systems.add<EnemyProjectileAnimationSystem>();
  systems.add<engine::systems::two_d::Physics>();
  systems.add<KnightAnimationSystem>();
  systems.add<KnightWalkingSystem>();
  systems.add<SpriteAnimator>();
  systems.add<TilemapRenderer>();
  systems.add<SpriteRenderer>();
  systems.add<ColliderRenderer>();
  systems.add<KnightAttackSystem>();
  systems.add<TurretAttackSystem>();
  systems.add<HealthSystem>();
  systems.add<ColorAnimator>();
  // systems.add<IgnoreCollisionSystem>(&entities, &events);
  systems.configure();
}

void Floor2D::Update(entityx::TimeDelta dt) {
  systems.update<PlayerInputSystem>(dt);
  systems.update<GhostIaSystem>(dt);
  systems.update<GhostAnimationSystem>(dt);
  systems.update<TurretIaSystem>(dt);
  systems.update<TrapIaSystem>(dt);
  systems.update<TurretWalkingSystem>(dt);
  systems.update<EnemyProjectileAnimationSystem>(dt);
  systems.update<engine::systems::two_d::Physics>(dt);
  systems.update<KnightAnimationSystem>(dt);
  systems.update<KnightWalkingSystem>(dt);
  systems.update<SpriteAnimator>(dt);
  systems.update<KnightAttackSystem>(dt);
  systems.update<TurretAttackSystem>(dt);
  systems.update<HealthSystem>(dt);
  systems.update<ColorAnimator>(dt);
  systems.update<TilemapRenderer>(dt);
  systems.update<SpriteRenderer>(dt);
  systems.update<ColliderRenderer>(dt);
  // systems.update<IgnoreCollisionSystem>(dt);
}

void Floor2D::OnPlayerEnteringDoor(Door entering_door) {
  entities.each<Camera, Transform>(
      [&](entityx::Entity entity, Camera& camera, Transform& transform) {
        glm::vec3 next_pos = transform.GetLocalPosition();
        if (entering_door.pos == "top") {
          next_pos += glm::vec3(0.0f, 288.0f, 0.0f);
        } else if (entering_door.pos == "bottom") {
          next_pos += glm::vec3(0.0f, -288.0f, 0.0f);
        } else if (entering_door.pos == "left") {
          next_pos += glm::vec3(-544.0f, 0.0f, 0.0f);
        } else if (entering_door.pos == "right") {
          next_pos += glm::vec3(544.0f, 0.0f, 0.0f);
        } else {
          // error
        }
        transform.SetLocalPosition(next_pos);
      });

  glm::vec3 next_position_player;

  entities.each<Door, Transform, AABBCollider>([&](entityx::Entity entity,
                                                   Door& door,
                                                   Transform& transform,
                                                   AABBCollider& collider) {
    if (entering_door.pos == "top" && door.pos == "bottom") {
      next_position_player = transform.GetLocalPosition() +
                             glm::vec3(0.0f, 9.0f + collider.half_size.y, 0.0f);
      return;
    } else if (entering_door.pos == "bottom" && door.pos == "top") {
      next_position_player =
          transform.GetLocalPosition() +
          glm::vec3(0.0f, -9.0f - collider.half_size.y, 0.0f);
      return;
    } else if (entering_door.pos == "left" && door.pos == "right") {
      next_position_player =
          transform.GetLocalPosition() +
          glm::vec3(-9.0f - collider.half_size.x, 0.0f, 0.0f);
      return;
    } else if (entering_door.pos == "right" && door.pos == "left") {
      next_position_player = transform.GetLocalPosition() +
                             glm::vec3(9.0f + collider.half_size.x, 0.0f, 0.0f);
      return;
    }
  });

  entities.each<Player, Transform>(
      [&](entityx::Entity entity, Player& player, Transform& transform) {
        transform.SetLocalPosition(next_position_player);
      });
}

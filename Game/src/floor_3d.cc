#include "floor_3d.h"

#include <engine/systems/three_d/model_renderer.h>
#include <engine/systems/two_d/collider_renderer.h>
#include <engine/systems/two_d/physics.h>

#include "systems.h"
#include "systems_3d.h"

Floor3D::Floor3D(Game *parent_scene) : Floor(parent_scene) {
  systems.add<PlayerInputSystem>();
  systems.add<CameraFollowPlayerSystem>();
  systems.add<GhostIaSystem>();
  systems.add<TurretIaSystem>();
  systems.add<TrapIaSystem>();
  systems.add<engine::systems::two_d::Physics>();
  systems.add<engine::systems::two_d::ColliderRenderer>();
  systems.add<engine::systems::three_d::ModelRenderer>();
  systems.add<KnightAttackSystem>();
  systems.add<WizardAttackSystem>();
  systems.add<TurretAttackSystem>();
  systems.add<HealthSystem>();
  systems.configure();
}

void Floor3D::Update(entityx::TimeDelta dt) {
  if(!paused){
    systems.update<PlayerInputSystem>(dt);
    systems.update<CameraFollowPlayerSystem>(dt);
    systems.update<GhostIaSystem>(dt);
    systems.update<TurretIaSystem>(dt);
    systems.update<TrapIaSystem>(dt);
    systems.update<engine::systems::two_d::Physics>(dt);
    systems.update<KnightAttackSystem>(dt);
    systems.update<WizardAttackSystem>(dt);
    systems.update<TurretAttackSystem>(dt);
    systems.update<HealthSystem>(dt);
    systems.update<engine::systems::three_d::ModelRenderer>(dt);
  }
  else{
    systems.update<PlayerInputSystem>(dt);
    systems.update<CameraFollowPlayerSystem>(dt);
    systems.update<GhostIaSystem>(0);
    systems.update<TurretIaSystem>(0);
    systems.update<TrapIaSystem>(0);
    systems.update<engine::systems::two_d::Physics>(0);
    systems.update<KnightAttackSystem>(0);
    systems.update<WizardAttackSystem>(0);
    systems.update<TurretAttackSystem>(0);
    systems.update<HealthSystem>(0);
    systems.update<engine::systems::three_d::ModelRenderer>(0);
  }
  }

void Floor3D::OnPlayerEnteringDoor(Door entering_door) {
  glm::vec3 next_position_player;
  static const float kDisplacement = 2.0f;
  entities.each<Door, engine::components::common::Transform,
                engine::components::two_d::AABBCollider>(
      [&](entityx::Entity entity, Door &door,
          engine::components::common::Transform &transform,
          engine::components::two_d::AABBCollider &collider) {
        if (entering_door.pos == "top" && door.pos == "bottom") {
          next_position_player =
              transform.GetLocalPosition() +
              glm::vec3(0.0f, kDisplacement + collider.half_size.y, 0.0f);
          return;
        } else if (entering_door.pos == "bottom" && door.pos == "top") {
          next_position_player =
              transform.GetLocalPosition() +
              glm::vec3(0.0f, -kDisplacement - collider.half_size.y, 0.0f);
          return;
        } else if (entering_door.pos == "left" && door.pos == "right") {
          next_position_player =
              transform.GetLocalPosition() +
              glm::vec3(-kDisplacement - collider.half_size.x, 0.0f, 0.0f);
          return;
        } else if (entering_door.pos == "right" && door.pos == "left") {
          next_position_player =
              transform.GetLocalPosition() +
              glm::vec3(kDisplacement + collider.half_size.x, 0.0f, 0.0f);
          return;
        }
      });
  entities.each<Player, engine::components::common::Transform>(
      [&](entityx::Entity entity, Player &player,
          engine::components::common::Transform &transform) {
        auto player_pos = transform.GetWorldPosition();
        transform.SetLocalPosition(glm::vec3(
            next_position_player.x, next_position_player.y, player_pos.z));
      });
}

void Floor3D::OnPlayerEnteringBossDoorWithKey(BossDoor entering_door) {}

void Floor3D::OnPlayerEnteringBossDoorWithoutKey() {}

void Floor3D::PauseGame(bool pause){
  paused = pause;
}

bool Floor3D::GetPaused(){
  return paused;
}
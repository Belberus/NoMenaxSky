#include "floor_3d.h"

#include <engine/systems/three_d/model_renderer.h>
#include <engine/systems/two_d/collider_renderer.h>
#include <engine/systems/two_d/physics.h>

#include "systems.h"

Floor3D::Floor3D(Game *parent_scene) : Floor(parent_scene) {
  systems.add<PlayerInputSystem>();
  systems.add<GhostIaSystem>();
  systems.add<TurretIaSystem>();
  systems.add<TrapIaSystem>();
  systems.add<engine::systems::two_d::Physics>();
  systems.add<KnightAnimationSystem>();
  systems.add<KnightWalkingSystem>();
  systems.add<engine::systems::two_d::ColliderRenderer>();
  //systems.add<engine::systems::three_d::ModelRenderer>();
  systems.add<KnightAttackSystem>();
  systems.add<TurretAttackSystem>();
  systems.add<HealthSystem>();
  systems.configure();
}

void Floor3D::Update(entityx::TimeDelta dt) {
  systems.update<PlayerInputSystem>(dt);
  systems.update<GhostIaSystem>(dt);
  systems.update<TurretIaSystem>(dt);
  systems.update<TrapIaSystem>(dt);
  systems.update<engine::systems::two_d::Physics>(dt);
  systems.update<KnightAttackSystem>(dt);
  systems.update<TurretAttackSystem>(dt);
  systems.update<HealthSystem>(dt);
  //systems.update<engine::systems::three_d::ModelRenderer>(dt);
  systems.update<engine::systems::two_d::ColliderRenderer>(dt);
}

void Floor3D::OnPlayerEnteringDoor(Door entering_door) {}

void Floor3D::OnPlayerEnteringBossDoorWithKey(BossDoor entering_door) {}

void Floor3D::OnPlayerEnteringBossDoorWithoutKey() {}

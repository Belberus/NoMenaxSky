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

#include <engine/core/audio_manager.h>
#include <engine/core/engine.h>

#include "components.h"
#include "floor.h"
#include "systems.h"
#include "events.h"

using namespace engine::core;
using namespace engine::systems::two_d;
using namespace engine::events;
using namespace engine::components::two_d;
using namespace engine::components::common;

Floor2D::Floor2D(Game* parent_scene) : Floor(parent_scene) {
  systems.add<PlayerInputSystem>();
  systems.add<GhostIaSystem>();
  systems.add<GhostAnimationSystem>();
  systems.add<MasiatrixAnimationSystem>();
  systems.add<ManuelethAnimationSystem>();
  systems.add<TurretIaSystem>();
  systems.add<ManuelethIaSystem>();
  systems.add<TrapIaSystem>();
  systems.add<LancerIaSystem>();
  systems.add<MasiatrixIaSystem>();
  systems.add<MenaxIaSystem>();
  systems.add<TurretWalkingSystem>();
  systems.add<LancerWalkingSystem>();
  systems.add<MasiatrixWalkingSystem>();
  systems.add<EnemyProjectileAnimationSystem>();
  systems.add<engine::systems::two_d::Physics>();
  systems.add<KnightAnimationSystem>();
  systems.add<WizardAnimationSystem>();
  systems.add<WizardAttackSystem>();
  systems.add<LancerAnimationSystem>();
  systems.add<KnightWalkingSystem>();
  systems.add<SpriteAnimator>();
  systems.add<TilemapRenderer>();
  systems.add<SpriteRenderer>();
  systems.add<ColliderRenderer>();
  systems.add<KnightAttackSystem>();
  systems.add<EnemyProjectileSystem>();
  systems.add<LancerAttackSystem>();
  systems.add<GhostAttackSystem>();
  systems.add<MenaxAttackSystem>();
  systems.add<ChestSystem>();
  systems.add<LeverSystem>();
  systems.add<HealthSystem>();
  systems.add<ColorAnimator>();
  systems.add<ShieldSystem>();
  systems.add<PauseInputSystem>();
  systems.add<MasiatrixBossFight>(&entities);
  systems.add<SpawnSystem>();
  systems.add<MenaxAnimationSystem>();
  systems.configure();
}

void Floor2D::Update(entityx::TimeDelta dt) {

  if(!paused){
    systems.update<PlayerInputSystem>(dt);
    systems.update<GhostIaSystem>(dt);
    systems.update<GhostAnimationSystem>(dt);
    systems.update<MasiatrixAnimationSystem>(dt);
    systems.update<ManuelethAnimationSystem>(dt);
    systems.update<TurretIaSystem>(dt);
    systems.update<MenaxAnimationSystem>(dt);
    systems.update<TurretIaSystem>(dt);
    systems.update<LancerIaSystem>(dt);
    systems.update<ManuelethIaSystem>(dt);
    systems.update<TrapIaSystem>(dt);
    systems.update<MasiatrixIaSystem>(dt);
    systems.update<MenaxIaSystem>(dt);
    systems.update<SpawnSystem>(dt);
    systems.update<TurretWalkingSystem>(dt);
    systems.update<LancerWalkingSystem>(dt);
    systems.update<MasiatrixWalkingSystem>(dt);
    systems.update<EnemyProjectileAnimationSystem>(dt);
    systems.update<engine::systems::two_d::Physics>(dt);
    systems.update<KnightAnimationSystem>(dt);
    systems.update<WizardAnimationSystem>(dt);
    systems.update<WizardAttackSystem>(dt);
    systems.update<LancerAnimationSystem>(dt);
    systems.update<KnightWalkingSystem>(dt);
    systems.update<SpriteAnimator>(dt);
    systems.update<KnightAttackSystem>(dt);
    systems.update<GhostAttackSystem>(dt);
    systems.update<MenaxAttackSystem>(dt);
    systems.update<ChestSystem>(dt);
    systems.update<LeverSystem>(dt);
    systems.update<EnemyProjectileSystem>(dt);
    systems.update<LancerAttackSystem>(dt);
    systems.update<HealthSystem>(dt);
    systems.update<MasiatrixBossFight>(dt);
    systems.update<ColorAnimator>(dt);
    systems.update<TilemapRenderer>(dt);
    systems.update<SpriteRenderer>(dt);
    systems.update<ColliderRenderer>(dt);
    systems.update<ShieldSystem>(dt);
    systems.update<PauseInputSystem>(dt);
    systems.update<LeverSystem>(dt);
  }
  else{ //is paused
    systems.update<PlayerInputSystem>(0);
    systems.update<GhostIaSystem>(0);
    systems.update<GhostAnimationSystem>(0);
    systems.update<MasiatrixAnimationSystem>(0);
    systems.update<ManuelethAnimationSystem>(0);
    systems.update<TurretIaSystem>(0);
    systems.update<MenaxAnimationSystem>(0);
    systems.update<TurretIaSystem>(0);
    systems.update<LancerIaSystem>(0);
    systems.update<ManuelethIaSystem>(0);
    systems.update<TrapIaSystem>(0);
    systems.update<MasiatrixIaSystem>(0);
    systems.update<MenaxIaSystem>(0);
    systems.update<SpawnSystem>(0);
    systems.update<TurretWalkingSystem>(0);
    systems.update<LancerWalkingSystem>(0);
    systems.update<MasiatrixWalkingSystem>(0);
    systems.update<EnemyProjectileAnimationSystem>(0);
    systems.update<engine::systems::two_d::Physics>(0);
    systems.update<KnightAnimationSystem>(0);
    systems.update<WizardAnimationSystem>(0);
    systems.update<WizardAttackSystem>(0);
    systems.update<LancerAnimationSystem>(0);
    systems.update<KnightWalkingSystem>(0);
    systems.update<GhostAttackSystem>(0);
    systems.update<MenaxAttackSystem>(0);
    systems.update<SpriteAnimator>(0);
    systems.update<KnightAttackSystem>(0);
    systems.update<ChestSystem>(0);
    systems.update<LeverSystem>(0);
    systems.update<EnemyProjectileSystem>(0);
    systems.update<LancerAttackSystem>(0);
    systems.update<HealthSystem>(0);
    systems.update<MasiatrixBossFight>(0);
    systems.update<ColorAnimator>(0);
    systems.update<TilemapRenderer>(0);
    systems.update<SpriteRenderer>(0);
    systems.update<ColliderRenderer>(0);
    systems.update<ShieldSystem>(0);
    systems.update<PauseInputSystem>(dt);
    systems.update<LeverSystem>(0);
  }
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

void Floor2D::OnPlayerEnteringBossDoorWithKey(BossDoor entering_door) {
    Engine::GetInstance().Get<AudioManager>().PlaySound(
          "assets/media/fx/defaults/boss_door.wav", false, 0.4f);
  if(GetLevel() == 1){
    PlayText pt("Este es el temible y poderoso mago Manueleth.\n\nCuidado con las bolas magicas que lanza y sus trampas.\n\nDerrotalo para avanzar al siguiente piso del Castillo de Menax.\n\n\n                    Pulsa [ENTER] para continuar.");
    GetParentScene()->events.emit<PlayText>(pt);
  }
  else if(GetLevel() == 2){
    PlayText pt("Esta es la legendaria Masiatrix, maestra del sigilo y las argucias.\n\nDeberas descubir cual es la copia verdadera y atacarla.\n\nDerrotala para avanzar al siguiente piso del Castillo de Menax.\n\n\n                    Pulsa [ENTER] para continuar.");
    GetParentScene()->events.emit<PlayText>(pt);
  }
  else if(GetLevel() == 3){
    PlayText pt("Finalmente, Lord Menax.\n\nTen cuidado, sera un enfrentamiento duro. No dejes que te salte encima\no estaras muerto antes de que te des cuenta.\n\nDerrotalo para recuperar tus queridas tartas!\n\n                    Pulsa [ENTER] para continuar.");
    GetParentScene()->events.emit<PlayText>(pt);
  }
  else{
    PlayText pt("Que haces aqui.\nSolo los devs pueden estar aqui.\nTu no eres un dev.");
    GetParentScene()->events.emit<PlayText>(pt);
  }
  
  entities.each<Camera, Transform>(
      [&](entityx::Entity entity, Camera& camera, Transform& transform) {
        glm::vec3 next_pos = transform.GetLocalPosition();
        if (entering_door.pos == "top") {
          next_pos += glm::vec3(0.0f, 288.0f, 0.0f);
        }
        transform.SetLocalPosition(next_pos);
      });

  glm::vec3 next_position_player;

  entities.each<BossDoor, Transform, AABBCollider>(
      [&](entityx::Entity entity, BossDoor& bossDoor, Transform& transform,
          AABBCollider& collider) {
        if (entering_door.pos == "top" && bossDoor.pos == "bottom") {
          next_position_player =
              transform.GetLocalPosition() +
              glm::vec3(0.0f, 9.0f + collider.half_size.y, 0.0f);
          return;
        }
      });

  entities.each<Player, Transform>(
      [&](entityx::Entity entity, Player& player, Transform& transform) {
        transform.SetLocalPosition(next_position_player);
      });
}

bool oneTime1=true;
bool oneTime2=true;
void Floor2D::OnPlayerEnteringBossDoorWithoutKey() {
  if(GetLevel() == 1){
    if (oneTime1) {
      oneTime1 = false;
      PlayText pt("Busca la llave situada en uno de los dos baules para poder entrar.\n\n\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
      GetParentScene()->events.emit<PlayText>(pt);
    }   
  }
  else if(GetLevel() == 2){
    if (oneTime2) {
      oneTime2 = false;
      PlayText pt("Activa las dos palancas para poder entrar.\n\n\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
      GetParentScene()->events.emit<PlayText>(pt);
    }
  }
  else{
    PlayText pt("Nunca deberia llegarse aqui");
    GetParentScene()->events.emit<PlayText>(pt);
  }
}

void Floor2D::PauseGame(bool pause){
  paused = pause;
}

bool Floor2D::GetPaused(){
  return paused;
}
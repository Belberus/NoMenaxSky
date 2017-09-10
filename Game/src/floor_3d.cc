#include "floor_3d.h"

#include <engine/systems/three_d/model_renderer.h>
#include <engine/systems/two_d/collider_renderer.h>
#include <engine/systems/two_d/physics.h>

#include <engine/core/audio_manager.h>
#include <engine/core/engine.h>

#include "systems.h"
#include "systems_3d.h"

using namespace engine::core;
using namespace engine::systems::two_d;
using namespace engine::events;

Floor3D::Floor3D(Game *parent_scene) : Floor(parent_scene) {
  systems.add<PlayerInputSystem>();
  systems.add<CameraFollowPlayerSystem>();
  systems.add<GhostIaSystem>();
  systems.add<TurretIaSystem>();
  systems.add<LancerIaSystem>();
  systems.add<ManuelethIaSystem>();
  systems.add<MasiatrixIaSystem>();
  systems.add<MasiatrixBossFight>(&entities);
  systems.add<TrapIaSystem>();
  systems.add<engine::systems::two_d::Physics>();
  systems.add<engine::systems::two_d::ColliderRenderer>();
  systems.add<engine::systems::three_d::ModelRenderer>();
  systems.add<KnightAttackSystem>();
  systems.add<ShieldSystem>(); //Alias energy system
  systems.add<ChestSystem>(); //Alias key system
  systems.add<LeverSystem>();
  systems.add<WizardAttackSystem>();
  systems.add<EnemyProjectileSystem>();
  systems.add<HealthSystem>();
  systems.add<PauseInputSystem>();
  systems.add<RotatePlayerSystem>();
  systems.configure();
}

void Floor3D::Update(entityx::TimeDelta dt) {

  if(!paused){
    systems.update<PlayerInputSystem>(dt);
    systems.update<GhostIaSystem>(dt);
    systems.update<TurretIaSystem>(dt);
    systems.update<LancerIaSystem>(dt);
    systems.update<ManuelethIaSystem>(dt);
    systems.update<MasiatrixIaSystem>(dt);
    systems.update<MasiatrixBossFight>(dt);
    systems.update<TrapIaSystem>(dt);
    systems.update<RotatePlayerSystem>(dt);
    systems.update<engine::systems::two_d::Physics>(dt);
    systems.update<CameraFollowPlayerSystem>(dt);
    systems.update<KnightAttackSystem>(dt);
    systems.update<WizardAttackSystem>(dt);
    systems.update<ShieldSystem>(dt);
    systems.update<ChestSystem>(dt);
    systems.update<LeverSystem>(dt);
    //systems.update<TurretAttackSystem>(dt);
    systems.update<HealthSystem>(dt);    
    systems.update<PauseInputSystem>(dt);
    systems.update<engine::systems::three_d::ModelRenderer>(dt);
    systems.update<engine::systems::two_d::ColliderRenderer>(dt);
  }
  else{
    systems.update<PlayerInputSystem>(dt);
    systems.update<CameraFollowPlayerSystem>(dt);
    systems.update<GhostIaSystem>(0);
    systems.update<TurretIaSystem>(0);
    systems.update<LancerIaSystem>(0);
    systems.update<ManuelethIaSystem>(0);
    systems.update<MasiatrixIaSystem>(0);
    systems.update<MasiatrixBossFight>(0);
    systems.update<TrapIaSystem>(0);
    systems.update<engine::systems::two_d::Physics>(0);
    systems.update<KnightAttackSystem>(0);
    systems.update<WizardAttackSystem>(0);
    systems.update<ShieldSystem>(0);
    systems.update<ChestSystem>(0);
    systems.update<LeverSystem>(0);
    systems.update<HealthSystem>(0);
    systems.update<PauseInputSystem>(dt);
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

void Floor3D::OnPlayerEnteringBossDoorWithKey(BossDoor entering_door) {
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

  glm::vec3 next_position_player;
  static const float kDisplacement = 2.0f;
  entities.each<BossDoor, engine::components::common::Transform,
                engine::components::two_d::AABBCollider>(
      [&](entityx::Entity entity, BossDoor &bossDoor,
          engine::components::common::Transform &transform,
          engine::components::two_d::AABBCollider &collider) {
        if(entering_door.pos == "top" && bossDoor.pos == "bottom"){
          next_position_player =
          transform.GetLocalPosition() +
          glm::vec3(0.0f, kDisplacement + collider.half_size.y, 0.0f);
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

bool oneTime3=true;
bool oneTime4=true;
void Floor3D::OnPlayerEnteringBossDoorWithoutKey() {
if(GetLevel() == 1){
    if (oneTime3) {
      oneTime3 = false;
      PlayText pt("Busca la llave situada en uno de los dos baules para poder entrar.\n\n\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
      GetParentScene()->events.emit<PlayText>(pt);
    }   
  }
  else if(GetLevel() == 2){
    if (oneTime4) {
      oneTime4 = false;
      PlayText pt("Activa las dos palancas para poder entrar.\n\n\n\n\n\n\n                    Pulsa [ENTER] para continuar.");
      GetParentScene()->events.emit<PlayText>(pt);
    }
  }
  else{
    PlayText pt("Nunca deberia llegarse aqui");
    GetParentScene()->events.emit<PlayText>(pt);
  }}

void Floor3D::PauseGame(bool pause){
  paused = pause;
}

bool Floor3D::GetPaused(){
  return paused;
}
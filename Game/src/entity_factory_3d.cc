#include "entity_factory_3d.h"

#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/components/three_d/model.h>
#include <engine/core/engine.h>
#include <engine/components/two_d/aabb_collider.h>

#include "components.h"

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::components::three_d;
using namespace engine::core;

std::vector<entityx::Entity> EntityFactory3D::MakeKnight(
    entityx::EntityManager& entities, const glm::vec3& position) {
  std::vector<entityx::Entity> entities_created;
  auto player = entities.create();
  player.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/caballero/caballero.dae");
  player.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  player.assign<engine::components::common::Transform>(
      position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  player.assign<Player>(Player::Orientation::DOWN);
  player.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(2.0f, 2.0f));
  player.assign<KnightAttack>(100, KnightAttack::Orientation::UP);
  player.assign<Energy>(100.0f, 100.0f);
  player.assign<Health>(300.0f, 300.0f, "assets/media/fx/gaunt/default/death.wav");
  player.assign<ThreeD>();
  entities_created.push_back(player);

  // adding sword entity
  auto sword_hitbox = entities.create();
  sword_hitbox.assign<engine::components::common::Transform>(
      glm::vec3(0.0f, 0.0f, 0.0f),
      player.component<engine::components::common::Transform>().get());
  sword_hitbox.assign<engine::components::two_d::AABBCollider>(
      glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), true);
  sword_hitbox.assign<engine::components::common::Physics>(
      glm::vec3(0.0f, 0.0f, 0.0f));
  MeleeWeapon weapon_cmp;
  weapon_cmp.damage = 10.0f;
  weapon_cmp.owner = player;
  weapon_cmp.drawn = false;
  sword_hitbox.assign<MeleeWeapon>(weapon_cmp);
  entities_created.push_back(sword_hitbox);
  // adding shield entity
  auto shield = entities.create();
  shield.assign<engine::components::common::Transform>(
      glm::vec3(0.0f, -9.0f, 0.0f),
      player.component<engine::components::common::Transform>().get());
  shield.assign<engine::components::two_d::AABBCollider>(
      glm::vec2(0.0f, 0.0f), glm::vec2(8.0f, 3.0f), true);
  shield.assign<engine::components::common::Physics>(
      glm::vec3(0.0f, 0.0f, 0.0f));
  shield.assign<Shield>(player);

  entities_created.push_back(shield);
  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeWizard(
    entityx::EntityManager& entities, const glm::vec3& position) {

  std::vector<entityx::Entity> entities_created;
  auto player = entities.create();
  player.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/mago/mago.dae");
  player.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  player.assign<engine::components::common::Transform>(
      position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  player.assign<Player>(Player::Orientation::DOWN);
  player.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(2.0f, 2.0f));
  player.assign<Wizard>();
  player.assign<Energy>(100.0f, 100.0f);
  player.assign<Health>(200.0f, 200.0f, "assets/media/fx/gaunt/default/death.wav");
  player.assign<ThreeD>();

  entities_created.push_back(player);

  return entities_created;

  return std::vector<entityx::Entity>();
}

std::vector<entityx::Entity> EntityFactory3D::MakeGhost(
    entityx::EntityManager& entities, const glm::vec3& position) {

  std::vector<entityx::Entity> entities_created;
  auto ghost = entities.create();
  ghost.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/fantasma/fantasma.dae");
  ghost.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  ghost.assign<engine::components::common::Transform>(
      position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  ghost.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 2.0f),
                                                         glm::vec2(3.0f, 4.0f));
  ghost.assign<Health>(10.0f, 10.0f, "assets/media/fx/ghost/default/death.wav");
  ghost.assign<Ghost>();
  entities_created.push_back(ghost);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeTurret(
    entityx::EntityManager& entities, const glm::vec3& position,
    const float frecuencia) {

  std::vector<entityx::Entity> entities_created;
  auto turret = entities.create();
  turret.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/torreta/torreta.dae");
  turret.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  turret.assign<engine::components::common::Transform>(
      position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  turret.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(5.0f, 10.0f));
  turret.assign<Health>(40.0f, 40.0f, "assets/media/fx/turret/default/death.wav");
  turret.assign<Turret>(frecuencia);
  entities_created.push_back(turret);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeTrap(
    entityx::EntityManager& entities, const glm::vec3& position,
    const std::string& orient, const float frecuencia) {

  std::vector<entityx::Entity> entities_created;
  auto trap = entities.create();
  trap.assign<engine::components::common::Transform>(
      position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  if (orient == "abajo") {
      trap.assign<Trap>(Trap::Orientation::DOWN, frecuencia);
    } else if (orient == "arriba") {
      trap.assign<Trap>(Trap::Orientation::UP, frecuencia);
    } else if (orient == "izquierda") {
      trap.assign<Trap>(Trap::Orientation::LEFT, frecuencia);
    } else if (orient == "derecha") {
      trap.assign<Trap>(Trap::Orientation::RIGHT, frecuencia);
    }
  entities_created.push_back(trap);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeManueleth(
    entityx::EntityManager& entities, const glm::vec3& position) {

  std::vector<entityx::Entity> entities_created;
  auto manueleth = entities.create();
  manueleth.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/manueleth/manueleth.dae");
  manueleth.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  manueleth.assign<engine::components::common::Transform>(
      position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  manueleth.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(5.0f, 10.0f));
  manueleth.assign<Health>(300.0f, 300.0f, "assets/media/fx/manueleth/default/death.wav");
  manueleth.assign<Manueleth>();
  entities_created.push_back(manueleth);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeMasiatrix(
    entityx::EntityManager& entities, const glm::vec3& position, const std::string &id) {
  return std::vector<entityx::Entity>();
}

std::vector<entityx::Entity> EntityFactory3D::MakeEnemyProjectile(
    entityx::EntityManager& entities, const glm::vec3& position,
    const float& rotation, const glm::vec3& velocity, const std::string& type) {
  
  std::vector<entityx::Entity> entities_created;
  auto enemyProjectile = entities.create();

  engine::components::common::Transform t(position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  //enemyProjectile.assign<Transform>(position);
  glm::quat model_rotation;
  model_rotation = glm::rotate(model_rotation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
  t.SetLocalOrientation(model_rotation);
  enemyProjectile.assign<Transform>(t);

  if(type == "torreta"){
    enemyProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_torreta.dae");
    enemyProjectile.assign<engine::components::common::Physics>(velocity);
    enemyProjectile.assign<engine::components::two_d::AABBCollider>(glm::vec2(3.0f, 0.0f),
                                                           glm::vec2(2.0f, 2.0f));
    enemyProjectile.assign<EnemyProjectile>(15.0f);
  }
  else if(type == "trampa"){
    enemyProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_trampa.dae");
    enemyProjectile.assign<engine::components::common::Physics>(velocity);
    enemyProjectile.assign<engine::components::two_d::AABBCollider>(glm::vec2(3.0f, 0.0f),
                                                           glm::vec2(2.0f, 2.0f));
    enemyProjectile.assign<EnemyProjectile>(20.0f);
  }
  else if(type == "manueleth"){
    enemyProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_mago.dae"); //hacer algo para diferenciar
    enemyProjectile.assign<engine::components::common::Physics>(velocity);
    enemyProjectile.assign<engine::components::two_d::AABBCollider>(glm::vec2(3.0f, 0.0f),
                                                           glm::vec2(2.0f, 2.0f));
    enemyProjectile.assign<EnemyProjectile>(20.0f);
  }
  
  entities_created.push_back(enemyProjectile);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeWizardProjectile(
    entityx::EntityManager& entities, const glm::vec3& position,
    const float& rotation, const glm::vec3& velocity, const std::string& type) {

  std::vector<entityx::Entity> entities_created;
  auto wizardProjectile = entities.create();

    Transform t(position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
    //enemyProjectile.assign<Transform>(position);
    glm::quat model_rotation;
    model_rotation = glm::rotate(model_rotation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    t.SetLocalOrientation(model_rotation);

  if(type == "normal"){
    wizardProjectile.assign<Transform>(t);
    wizardProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_mago.dae");
    wizardProjectile.assign<engine::components::common::Physics>(velocity);
    wizardProjectile.assign<engine::components::two_d::AABBCollider>(glm::vec2(3.0f, 0.0f),
                                                           glm::vec2(2.0f, 2.0f));
    wizardProjectile.assign<WizardProjectile>(10.0f);
  }
  else if(type == "special"){
    t.SetLocalScale(glm::vec3(0.2,0.4,0.4));
    wizardProjectile.assign<Transform>(t);
    wizardProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_mago.dae");
    wizardProjectile.assign<engine::components::common::Physics>(velocity);
    wizardProjectile.assign<engine::components::two_d::AABBCollider>(glm::vec2(3.0f, 0.0f),
                                                           glm::vec2(2.0f, 2.0f));
    wizardProjectile.assign<WizardProjectile>(30.0f);
  }
  entities_created.push_back(wizardProjectile);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeLancer(
    entityx::EntityManager& entities, const glm::vec3& position) {

  std::vector<entityx::Entity> entities_created;
  auto lancer = entities.create();
  lancer.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/lancero/lancero.dae");
  lancer.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  lancer.assign<engine::components::common::Transform>(
      position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  lancer.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(7.0f, 7.0f));
  lancer.assign<Health>(30.0f, 30.0f, "assets/media/fx/lanc/default/death.wav");
  lancer.assign<Lancer>();
  entities_created.push_back(lancer);

  return entities_created;
}

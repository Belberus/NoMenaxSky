#include "entity_factory_3d.h"

#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/components/three_d/model.h>
#include <engine/components/two_d/aabb_collider.h>
#include <engine/core/engine.h>

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
      "assets/3d/personajes/caballero/caballero.fbx");
  player.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  engine::components::common::Transform transform(position, nullptr,
                                                  glm::vec3(0.2f, 0.2f, 0.2f));
  //glm::quat rotation;
  //rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  //transform.SetLocalOrientation(rotation);;
  player.assign<engine::components::common::Transform>(transform);
  player.assign<Player>(Player::Orientation::DOWN);
  player.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(1.0f, 1.0));
  player.assign<KnightAttack>(100, KnightAttack::Orientation::UP);
  player.assign<Energy>(100.0f, 100.0f);
  player.assign<Health>(300.0f, 300.0f,
                        "assets/media/fx/gaunt/default/death.wav");
  player.assign<ThreeD>();
  entities_created.push_back(player);

  // adding sword entity
  auto sword_hitbox = entities.create();
  sword_hitbox.assign<engine::components::common::Transform>(
      glm::vec3(0.0f, 0.0f, 0.0f),
      player.component<engine::components::common::Transform>().get());
  sword_hitbox.assign<engine::components::two_d::AABBCollider>(
      glm::vec2(0.0f, 0.0f), glm::vec2(0.3f, 0.3f), true);
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
      glm::vec2(0.0f, 0.0f), glm::vec2(2.4f, 0.9f), true);
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
      "assets/3d/personajes/mago/mago.fbx");
  player.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  player.assign<engine::components::common::Transform>(
      position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  player.assign<Player>(Player::Orientation::LEFT);
  player.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(1.0f, 1.0f));
  player.assign<Wizard>();
  player.assign<Energy>(100.0f, 100.0f);
  player.assign<Health>(200.0f, 200.0f,
                        "assets/media/fx/gaunt/default/death.wav");
  player.assign<ThreeD>();

  entities_created.push_back(player);

  return entities_created;

  return std::vector<entityx::Entity>();
}

std::vector<entityx::Entity> EntityFactory3D::MakeGhost(
    entityx::EntityManager& entities, const glm::vec3& position) {
  glm::vec3 newPosition = position;
  newPosition.z += 8.0f;
  std::vector<entityx::Entity> entities_created;
  auto ghost = entities.create();
  ghost.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/fantasma/fantasma.dae");
  ghost.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  Transform t(newPosition, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  glm::quat rot;
  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  t.SetLocalOrientation(rot);
  ghost.assign<engine::components::common::Transform>(t);
  ghost.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                        glm::vec2(1.0f, 1.0f));
  ghost.assign<Health>(10.0f, 10.0f, "assets/media/fx/ghost/default/death.wav");
  ghost.assign<Ghost>();
  ghost.assign<ThreeD>();
  entities_created.push_back(ghost);

  //hitbox
  auto attack_hitbox = entities.create();
  attack_hitbox.assign<GhostHitBox>(1.0f, ghost);
  attack_hitbox.assign<Transform>(glm::vec3(0.0f, 0.0f, 0.0f),
                                  ghost.component<Transform>().get());
  attack_hitbox.assign<AABBCollider>(glm::vec2(0.0f, 0.0f),
                                     glm::vec2(1.7f, 1.7f), true);
  attack_hitbox.assign<Physics>(glm::vec3(0.0f, 0.0f, 0.0f));
  entities_created.push_back(attack_hitbox);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeTurret(
    entityx::EntityManager& entities, const glm::vec3& position,
    const float frecuencia) {
  glm::vec3 newPosition = position;
  newPosition.z += 7.0f;
  std::vector<entityx::Entity> entities_created;
  auto turret = entities.create();
  turret.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/torreta/torreta.dae");
  turret.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  turret.assign<engine::components::common::Transform>(
      newPosition, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  turret.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(1.5f, 1.5f));
  turret.assign<Health>(40.0f, 40.0f,
                        "assets/media/fx/turret/default/death.wav");
  turret.assign<Turret>(frecuencia);
  turret.assign<ThreeD>();
  entities_created.push_back(turret);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeTrap(
    entityx::EntityManager& entities, const glm::vec3& position,
    const std::string& orient, const float frecuencia) {
  glm::vec3 newPosition = position;
  newPosition.z += 7.0f;

  std::vector<entityx::Entity> entities_created;
  auto trap = entities.create();
  trap.assign<engine::components::common::Transform>(
      newPosition, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  trap.assign<ThreeD>();
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
  glm::vec3 newPosition = position;
  newPosition.z += 7.0f;
  std::vector<entityx::Entity> entities_created;
  auto manueleth = entities.create();
  manueleth.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/manueleth/manueleth.dae");
  manueleth.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  Transform t(newPosition, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  glm::quat rot;
  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
  t.SetLocalOrientation(rot);
  manueleth.assign<engine::components::common::Transform>(t);

  manueleth.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(1.5f, 1.5f));
  manueleth.assign<Health>(250.0f, 250.0f, "assets/media/fx/manueleth/default/death.wav");
  manueleth.assign<Manueleth>();
  manueleth.assign<ThreeD>();
  entities_created.push_back(manueleth);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeMenax(
    entityx::EntityManager& entities, const glm::vec3& position) {
  glm::vec3 newPosition = position;
  newPosition.z += 7.0f;
  std::vector<entityx::Entity> entities_created;
  auto menax = entities.create();
  entityx::Entity menaxHitBox = entities.create();

  menax.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/menax/menax.dae");
  menax.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  Transform t(newPosition, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  glm::quat rot;
  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
  t.SetLocalOrientation(rot);
  menax.assign<engine::components::common::Transform>(t);
  menax.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                        glm::vec2(4.0f, 4.0f));
  menax.assign<Health>(600.0f, 600.0f,
                       "assets/media/fx/menax/default/death.wav");
  menax.assign<Menax>(newPosition, menaxHitBox);
  menax.assign<ThreeD>();

  menaxHitBox.assign<MenaxHitBox>(75.0f, menax);
  menaxHitBox.assign<Transform>(glm::vec3(0.0f, 0.0f, 0.0f),
                                menax.component<Transform>().get());
  menaxHitBox.assign<AABBCollider>(glm::vec2(0.0f, 0.0f), glm::vec2(8.0f, 8.0f),
                                   true);
  menaxHitBox.assign<Physics>(glm::vec3(0.0f, 0.0f, 0.0f));

  entities_created.push_back(menax);
  entities_created.push_back(menaxHitBox);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeSpawn(
    entityx::EntityManager& entities, const glm::vec3& position) {
  glm::vec3 newPosition = position;
  newPosition.z += 7.0f;
  std::vector<entityx::Entity> entities_created;
  entityx::Entity spawn = entities.create();
  spawn.assign<Transform>(newPosition);
  spawn.assign<Spawn>();
  entities_created.push_back(spawn);
  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeMasiatrix(
    entityx::EntityManager& entities, const glm::vec3& position,
    const std::string& id, const bool& real) {
  glm::vec3 newPosition = position;
  newPosition.z += 7.0f;
  std::vector<entityx::Entity> entities_created;
  auto masiatrix = entities.create();
  masiatrix.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/masiatrix/masiatrix.dae");
  masiatrix.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  Transform t(newPosition, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  glm::quat rot;
  rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, -1.0f));
  t.SetLocalOrientation(rot);
  masiatrix.assign<engine::components::common::Transform>(t);
  masiatrix.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(2.2f, 2.2f));
  //masiatrix.assign<Health>(150.0f, 150.0f, "assets/media/fx/masiatrix/default/death.wav");
  if(real){
    masiatrix.assign<Health>(150.0f, 150.0f, "assets/media/fx/masiatrix/default/death.wav");
  }
  else{
    masiatrix.assign<Health>(150.0f, 150.0f, "assets/media/fx/masiatrix/default/fake_death.wav");
  }
  masiatrix.assign<Masiatrix>(id,position,real);
  masiatrix.assign<ThreeD>();
  entities_created.push_back(masiatrix);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeEnemyProjectile(
    entityx::EntityManager& entities, const glm::vec3& position,
    const float& rotation, const glm::vec3& velocity, const std::string& type) {
  glm::vec3 newPosition = position;
  newPosition.z += 3.0f;

  std::vector<entityx::Entity> entities_created;
  auto enemyProjectile = entities.create();

  engine::components::common::Transform t(newPosition, nullptr,
                                          glm::vec3(0.2f, 0.2f, 0.2f));
  // enemyProjectile.assign<Transform>(position);
  glm::quat model_rotation;
  model_rotation =
      glm::rotate(model_rotation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
  t.SetLocalOrientation(model_rotation);
  enemyProjectile.assign<Transform>(t);

  if (type == "torreta") {
    enemyProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_torreta.dae");
    enemyProjectile.assign<engine::components::common::Physics>(velocity);
    enemyProjectile.assign<engine::components::two_d::AABBCollider>(
        glm::vec2(0.0f, 0.0f), glm::vec2(0.6f, 0.6f));
    enemyProjectile.assign<EnemyProjectile>(15.0f);
  } else if (type == "trampa") {
    enemyProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_trampa.dae");
    enemyProjectile.assign<engine::components::common::Physics>(velocity);
    enemyProjectile.assign<engine::components::two_d::AABBCollider>(
        glm::vec2(0.0f, 0.0f), glm::vec2(0.6f, 0.6f));
    enemyProjectile.assign<EnemyProjectile>(20.0f);
  } else if (type == "manueleth") {
    enemyProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_mago.dae");  // hacer algo para
                                                      // diferenciar
    enemyProjectile.assign<engine::components::common::Physics>(velocity);
    enemyProjectile.assign<engine::components::two_d::AABBCollider>(
        glm::vec2(0.0f, 0.0f), glm::vec2(0.6f, 0.6f));
    enemyProjectile.assign<EnemyProjectile>(20.0f);
  } else if (type == "masiatrix") {
    enemyProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_trampa.dae");  // hacer algo para
                                                        // diferenciar
    enemyProjectile.assign<engine::components::common::Physics>(velocity);
    enemyProjectile.assign<engine::components::two_d::AABBCollider>(
        glm::vec2(0.0f, 0.0f), glm::vec2(0.6f, 0.6f));
    enemyProjectile.assign<EnemyProjectile>(20.0f);
  }

  entities_created.push_back(enemyProjectile);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeWizardProjectile(
    entityx::EntityManager& entities, const glm::vec3& position,
    const float& rotation, const glm::vec3& velocity, const std::string& type) {
  glm::vec3 newPosition = position;
  newPosition.z += 9.0f;

  std::vector<entityx::Entity> entities_created;
  auto wizardProjectile = entities.create();

  Transform t(position, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  // enemyProjectile.assign<Transform>(position);
  glm::quat model_rotation;
  model_rotation =
      glm::rotate(model_rotation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
  t.SetLocalOrientation(model_rotation);

  if (type == "normal") {
    wizardProjectile.assign<Transform>(t);
    wizardProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_mago.dae");
    wizardProjectile.assign<engine::components::common::Physics>(velocity);
    wizardProjectile.assign<engine::components::two_d::AABBCollider>(
        glm::vec2(0.0f, 0.0f), glm::vec2(0.6f, 0.6f));
    wizardProjectile.assign<WizardProjectile>(10.0f);
  } else if (type == "special") {
    t.SetLocalScale(glm::vec3(0.2, 0.4, 0.4));
    wizardProjectile.assign<Transform>(t);
    wizardProjectile.assign<engine::components::three_d::Model>(
        "assets/3d/proyectiles/proyectil_mago.dae");
    wizardProjectile.assign<engine::components::common::Physics>(velocity);
    wizardProjectile.assign<engine::components::two_d::AABBCollider>(
        glm::vec2(0.0f, 0.0f), glm::vec2(0.8f, 0.8f));
    wizardProjectile.assign<WizardProjectile>(30.0f);
  }
  entities_created.push_back(wizardProjectile);

  return entities_created;
}

std::vector<entityx::Entity> EntityFactory3D::MakeLancer(
    entityx::EntityManager& entities, const glm::vec3& position) {
  glm::vec3 newPosition = position;
  newPosition.z += 7.0f;

  std::vector<entityx::Entity> entities_created;
  auto lancer = entities.create();
  lancer.assign<engine::components::three_d::Model>(
      "assets/3d/personajes/lancero/lancero.dae");
  lancer.assign<engine::components::common::Physics>(glm::vec3(0, 0, 0));
  Transform t(newPosition, nullptr, glm::vec3(0.2f, 0.2f, 0.2f));
  glm::quat rot;
  rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  t.SetLocalOrientation(rot);

  lancer.assign<engine::components::common::Transform>(t);
  lancer.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(2.1f, 2.1f));
  lancer.assign<Health>(30.0f, 30.0f, "assets/media/fx/lanc/default/death.wav");
  lancer.assign<Lancer>();
  lancer.assign<ThreeD>();

  // adding hitbox
  auto lanc_hitbox = entities.create();
  lanc_hitbox.assign<Transform>(glm::vec3(0.0f, 0.0f, 0.0f),
                                 lancer.component<Transform>().get());
  lanc_hitbox.assign<AABBCollider>(glm::vec2(0.0f, 0.0f),
                                    glm::vec2(4.0f, 4.0f), true);
  lanc_hitbox.assign<Physics>(glm::vec3(0.0f, 0.0f, 0.0f));
  lanc_hitbox.assign<LancerHitBox>(2.0f, lancer);
  entities_created.push_back(lanc_hitbox);
  entities_created.push_back(lancer);

  return entities_created;
}

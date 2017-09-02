#include "entity_factory_3d.h"

#include <engine/components/common/physics.h>
#include <engine/components/common/transform.h>
#include <engine/components/three_d/model.h>
#include <engine/components/two_d/aabb_collider.h>

#include "components.h"

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
  player.assign<Health>(200.0f, 200.0f, "assets/media/fx/gaunt/default/death.wav");
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

std::vector<entityx::Entity> EntityFactory3D::MakeGhost(
    entityx::EntityManager& entities, const glm::vec3& position) {
  return std::vector<entityx::Entity>();
}

std::vector<entityx::Entity> EntityFactory3D::MakeTurret(
    entityx::EntityManager& entities, const glm::vec3& position,
    const float frecuencia) {
  return std::vector<entityx::Entity>();
}

std::vector<entityx::Entity> EntityFactory3D::MakeTrap(
    entityx::EntityManager& entities, const glm::vec3& position,
    const std::string& orient, const float frecuencia) {
  return std::vector<entityx::Entity>();
}

std::vector<entityx::Entity> EntityFactory3D::MakeManueleth(
    entityx::EntityManager& entities, const glm::vec3& position) {
  return std::vector<entityx::Entity>();
}

std::vector<entityx::Entity> EntityFactory3D::MakeEnemyProjectile(
    entityx::EntityManager& entities, const glm::vec3& position,
    const float& rotation, const glm::vec3& velocity, const std::string& type) {
  return std::vector<entityx::Entity>();
}

std::vector<entityx::Entity> EntityFactory3D::MakeLancer(
    entityx::EntityManager& entities, const glm::vec3& position) {
  return std::vector<entityx::Entity>();
}

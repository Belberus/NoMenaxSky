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
  player.assign<engine::components::common::Transform>(position);
  player.assign<Player>(Player::Orientation::DOWN);
  player.assign<engine::components::two_d::AABBCollider>(glm::vec2(0.0f, 0.0f),
                                                         glm::vec2(2.0f, 2.0f));
  player.assign<KnightAttack>(100, KnightAttack::Orientation::UP);
  player.assign<Health>(100.0f, "assets/media/fx/gaunt/default/death.wav");
  entities_created.push_back(player);

  // adding sword entity
  auto sword_hitbox = entities.create();
  sword_hitbox.assign<engine::components::common::Transform>(
      glm::vec3(0.0f, -9.0f, 0.0f),
      player.component<engine::components::common::Transform>().get());
  sword_hitbox.assign<engine::components::two_d::AABBCollider>(
      glm::vec2(0.0f, 0.0f), glm::vec2(6.0f, 6.0f), true);
  sword_hitbox.assign<engine::components::common::Physics>(
      glm::vec3(0.0f, 0.0f, 0.0f));
  MeleeWeapon weapon_cmp;
  weapon_cmp.damage = 10.0f;
  weapon_cmp.owner = player;
  weapon_cmp.drawn = false;
  sword_hitbox.assign<MeleeWeapon>(weapon_cmp);
  entities_created.push_back(sword_hitbox);
  return entities_created;
}

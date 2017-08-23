#include "floor.h"

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
#include "systems.h"

using namespace engine::systems::two_d;
using namespace engine::events;
using namespace engine::components::two_d;
using namespace engine::components::common;

Floor::Floor() {
  events.subscribe<Collision>(*this);
  systems.add<PlayerInputSystem>();
  systems.add<GhostIaSystem>();
  systems.add<engine::systems::two_d::Physics>();
  systems.add<KnightAnimationSystem>();
  systems.add<SpriteAnimator>();
  systems.add<TilemapRenderer>();
  systems.add<SpriteRenderer>();
  systems.add<ColliderRenderer>();
  systems.add<KnightAttackSystem>();
  systems.add<HealthSystem>();
  systems.add<ColorAnimator>();
  systems.configure();
}

Floor::~Floor() {}

void Floor::Update(entityx::TimeDelta dt) {
  systems.update<PlayerInputSystem>(dt);
  systems.update<GhostIaSystem>(dt);
  systems.update<engine::systems::two_d::Physics>(dt);
  systems.update<KnightAnimationSystem>(dt);
  systems.update<SpriteAnimator>(dt);
  systems.update<KnightAttackSystem>(dt);
  systems.update<HealthSystem>(dt);
  systems.update<ColorAnimator>(dt);
  systems.update<TilemapRenderer>(dt);
  systems.update<SpriteRenderer>(dt);
  systems.update<ColliderRenderer>(dt);
}

void Floor::receive(const Collision& collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto player = collision_copy.e0.component<Player>();
  auto door = collision_copy.e1.component<Door>();
  if (door && player) {
    if (IsEntityTryingToCrossDoor(collision_copy.e0, collision_copy.e1)) {
      Door previous_door(*door);
      rooms_[current_room_]->Unload(*this);
      current_room_ = previous_door.next_door;
      rooms_[current_room_]->Load(*this);
      entities.each<Camera, Transform>(
          [&](entityx::Entity entity, Camera& camera, Transform& transform) {
            glm::vec3 next_pos = transform.GetLocalPosition();
            if (previous_door.pos == "top") {
              next_pos += glm::vec3(0.0f, 288.0f, 0.0f);
            } else if (previous_door.pos == "bottom") {
              next_pos += glm::vec3(0.0f, -288.0f, 0.0f);
            } else if (previous_door.pos == "left") {
              next_pos += glm::vec3(-544.0f, 0.0f, 0.0f);
            } else if (previous_door.pos == "right") {
              next_pos += glm::vec3(544.0f, 0.0f, 0.0f);
            } else {
              // error
            }
            transform.SetLocalPosition(next_pos);
          });

      glm::vec3 next_position_player;

      entities.each<Door, Transform, AABBCollider>(
          [&](entityx::Entity entity, Door& door, Transform& transform,
              AABBCollider& collider) {
            if (previous_door.pos == "top" && door.pos == "bottom") {
              next_position_player =
                  transform.GetLocalPosition() +
                  glm::vec3(0.0f, 9.0f + collider.half_size.y, 0.0f);
              return;
            } else if (previous_door.pos == "bottom" && door.pos == "top") {
              next_position_player =
                  transform.GetLocalPosition() +
                  glm::vec3(0.0f, -9.0f - collider.half_size.y, 0.0f);
              return;
            } else if (previous_door.pos == "left" && door.pos == "right") {
              next_position_player =
                  transform.GetLocalPosition() +
                  glm::vec3(-9.0f - collider.half_size.x, 0.0f, 0.0f);
              return;
            } else if (previous_door.pos == "right" && door.pos == "left") {
              next_position_player =
                  transform.GetLocalPosition() +
                  glm::vec3(9.0f + collider.half_size.x, 0.0f, 0.0f);
              return;
            }
          });

      entities.each<Player, Transform>(
          [&](entityx::Entity entity, Player& player, Transform& transform) {
            transform.SetLocalPosition(next_position_player);
          });
    }
  }
}

bool Floor::IsEntityTryingToCrossDoor(entityx::Entity crossing_entity,
                                      entityx::Entity door) {
  auto crossing_entity_velocity =
      crossing_entity.component<engine::components::common::Physics>()
          ->velocity;
  auto door_component = *door.component<Door>();

  auto trying_cross_left_door =
      door_component.pos == "left" && crossing_entity_velocity.x < 0.0f;
  auto trying_cross_right_door =
      door_component.pos == "right" && crossing_entity_velocity.x > 0.0f;
  auto trying_cross_top_door =
      door_component.pos == "top" && crossing_entity_velocity.y > 0.0f;
  auto trying_cross_bottom_door =
      door_component.pos == "bottom" && crossing_entity_velocity.y < 0.0f;

  return trying_cross_left_door || trying_cross_right_door ||
         trying_cross_top_door || trying_cross_bottom_door;
}

Floor::Room::Room() : entity_creators_() {}

void Floor::Room::Load(Floor& floor) {
  for (auto& fn : entity_creators_) {
    entityx::Entity id = floor.entities.create();
    auto new_entites = fn(floor.entities);
    created_entities_.insert(created_entities_.end(), new_entites.begin(),
                             new_entites.end());
  }
}

void Floor::Room::Unload(Floor& floor) {
  for (auto& e : created_entities_) {
    if (e.valid()) {
      e.destroy();
    }
  }
  created_entities_.clear();
}

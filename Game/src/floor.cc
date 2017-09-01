#include "floor.h"

#include <engine/events/collision.h>

Floor::Floor() { events.subscribe<engine::events::Collision>(*this); }

Floor::~Floor() = default;

void Floor::receive(const engine::events::Collision& collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto player = collision_copy.e0.component<Player>();
  auto door = collision_copy.e1.component<Door>();
  if (door && player &&
      IsEntityTryingToCrossDoor(collision_copy.e0, collision_copy.e1)) {
    Door previous_door(*door);
    rooms_[current_room_]->Unload(*this);
    current_room_ = previous_door.next_door;
    rooms_[current_room_]->Load(*this);
    OnPlayerEnteringDoor(previous_door);
  }
}

bool Floor::IsEntityTryingToCrossDoor(entityx::Entity crossing_entity,
                                      entityx::Entity door) {
  Player::Orientation crossing_entity_orientation =
      crossing_entity.component<Player>()->orientation;
  auto door_component = *door.component<Door>();

  auto trying_cross_left_door =
      door_component.pos == "left" &&
      (crossing_entity_orientation == Player::Orientation::LEFT);
  auto trying_cross_right_door =
      door_component.pos == "right" &&
      (crossing_entity_orientation == Player::Orientation::RIGHT);
  auto trying_cross_top_door =
      door_component.pos == "top" &&
      (crossing_entity_orientation == Player::Orientation::UP);
  auto trying_cross_bottom_door =
      door_component.pos == "bottom" &&
      (crossing_entity_orientation == Player::Orientation::DOWN);

  return trying_cross_left_door || trying_cross_right_door ||
         trying_cross_top_door || trying_cross_bottom_door;
}

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

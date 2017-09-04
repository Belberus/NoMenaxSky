#include "floor.h"

#include <engine/events/collision.h>

Floor::~Floor() = default;

Floor::Floor(Game* parent_scene) : parent_scene_(parent_scene) {
  events.subscribe<engine::events::Collision>(*this);
  events.subscribe<Health>(*this);
  events.subscribe<Energy>(*this);
  events.subscribe<PauseMenuEvent>(*this);
  events.subscribe<BackToGame>(*this);
  events.subscribe<StartLevel2>(*this);
  events.subscribe<Player>(*this);
  events.subscribe<Death>(*this);
  events.subscribe<PauseGameEvent>(*this);
  events.subscribe<UnpauseGameEvent>(*this);
  events.subscribe<PlayText>(*this);
}

void Floor::receive(const PlayText& pt){
  parent_scene_->events.emit<PlayText>(pt);
}

void Floor::receive(const Player& player) {
  parent_scene_->events.emit<Player>(player);
}

void Floor::receive(const PauseMenuEvent& pm){
  PauseGame(true);
  parent_scene_->events.emit<PauseMenuEvent>(pm);
}

void Floor::receive(const PauseGameEvent& pg){
  PauseGame(true);
}

void Floor::receive(const UnpauseGameEvent& upg){
  PauseGame(false);
  parent_scene_->events.emit<BackToGame>();
}

void Floor::receive(const BackToGame& event){
  PauseGame(false);
  parent_scene_->events.emit<BackToGame>(event);
}

void Floor::receive(const StartLevel2& event) {
  parent_scene_->events.emit<StartLevel2>();
}

void Floor::receive(const Health& health) {
  parent_scene_->events.emit<Health>(health);
}

void Floor::receive(const Energy& energy) {
  parent_scene_->events.emit<Energy>(energy);
}

void Floor::receive(const Death& death) {
  parent_scene_->events.emit<Death>();
}

void Floor::receive(const engine::events::Collision& collision) {
  auto collision_copy = collision;
  if (!collision_copy.e0.valid() || !collision_copy.e1.valid()) {
    return;
  }
  auto player = collision_copy.e0.component<Player>();
  auto door = collision_copy.e1.component<Door>();
  auto bossDoor = collision_copy.e1.component<BossDoor>();

  int enemies_in_the_room = 0;
  for (auto e : entities.entities_with_components<Ghost>()) {
    enemies_in_the_room++;
  }
  for (auto e : entities.entities_with_components<Turret>()) {
    enemies_in_the_room++;
  }
  for (auto e : entities.entities_with_components<Lancer>()) {
    enemies_in_the_room++;
  }
  for (auto e : entities.entities_with_components<Manueleth>()) {
    enemies_in_the_room++;
  }
  if (door && player && enemies_in_the_room == 0) {
    if (IsEntityTryingToCrossDoor(collision_copy.e0, collision_copy.e1)) {
      rooms_[current_room_]->visited = true;
      Door previous_door(*door);
      rooms_[current_room_]->Unload(*this);
      current_room_ = previous_door.next_door;
      rooms_[current_room_]->Load(*this);
      OnPlayerEnteringDoor(previous_door);
    }
  } else if (bossDoor && player && enemies_in_the_room == 0) {
    // Con puerta de boss
    auto bossDoor = collision_copy.e1.component<BossDoor>();
    auto player = collision_copy.e0.component<Player>();
    if (IsEntityTryingToCrossBossDoor(collision_copy.e0, collision_copy.e1)) {
      if (player->key == true) {
        rooms_[current_room_]->visited = true;
        BossDoor previous_door(*bossDoor);
        rooms_[current_room_]->Unload(*this);
        current_room_ = previous_door.next_door;
        rooms_[current_room_]->Load(*this);
        OnPlayerEnteringBossDoorWithKey(previous_door);
      } else {
        OnPlayerEnteringBossDoorWithoutKey();
      }
    }
  }
}

bool Floor::IsEntityTryingToCrossBossDoor(entityx::Entity crossing_entity,
                                          entityx::Entity bossDoor) {
  Player::Orientation crossing_entity_orientation =
      crossing_entity.component<Player>()->orientation;
  auto bossDoor_component = *bossDoor.component<BossDoor>();
  auto trying_cross_bossDoor =
      bossDoor_component.pos == "top" &&
      (crossing_entity_orientation == Player::Orientation::UP);
  return trying_cross_bossDoor;
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
    if(visited){
      //entityx::Entity id = floor.entities.create();
      auto new_entites = fn(floor.entities);
      for(auto &e : new_entites){
        if((e.component<Health>()) || (e.component<TurretLegs>())
          || (e.component<LancerLegs>()) || (e.component<GhostHitBox>())
          || (e.component<LancerHitBox>())){
          e.destroy();
        }
      }
      created_entities_.insert(created_entities_.end(), new_entites.begin(),
                               new_entites.end());
    }
    else{ //load normally
      entityx::Entity id = floor.entities.create();
      auto new_entites = fn(floor.entities);
      created_entities_.insert(created_entities_.end(), new_entites.begin(),
                             new_entites.end());
    }
    
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

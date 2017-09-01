#ifndef FLOOR_H_
#define FLOOR_H_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <engine/core/scene.h>
#include <engine/events/collision.h>
#include <entityx/entityx.h>

#include "game.h"

class Floor : public engine::core::Scene, public entityx::Receiver<Floor> {
 public:
  Floor(Game *parent_scene);
  virtual ~Floor();
  void Update(entityx::TimeDelta dt) override;
  void receive(const engine::events::Collision &collision);
  void receive(const Health &health);

 private:
  Game *parent_scene_;
  class Room {
   public:
    Room();
    void Load(Floor &floor);
    void Unload(Floor &floor);
    std::vector<
        std::function<std::vector<entityx::Entity>(entityx::EntityManager &)>>
        entity_creators_;
    std::vector<entityx::Entity> created_entities_;
  };

  static bool IsEntityTryingToCrossDoor(entityx::Entity crossing_entity,
                                        entityx::Entity door);
  static bool IsEntityTryingToCrossBossDoor(entityx::Entity crossing_entity,
                                        entityx::Entity door);

  std::string current_room_;
  std::unordered_map<std::string, std::unique_ptr<Floor::Room>> rooms_;
  friend class FloorFactory;
};
#endif  // FLOOR_H_

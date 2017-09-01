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

#include "components.h"

class Floor : public engine::core::Scene, public entityx::Receiver<Floor> {
 public:
  Floor();
  virtual ~Floor();
  virtual void OnPlayerEnteringDoor(Door entering_door) = 0;
  void receive(const engine::events::Collision &collision);

 protected:
  class Room {
   public:
    void Load(Floor &floor);
    void Unload(Floor &floor);
    std::vector<
        std::function<std::vector<entityx::Entity>(entityx::EntityManager &)>>
        entity_creators_;
    std::vector<entityx::Entity> created_entities_;
  };

  static bool IsEntityTryingToCrossDoor(entityx::Entity crossing_entity,
                                        entityx::Entity door);

  std::string current_room_;
  std::unordered_map<std::string, std::unique_ptr<Floor::Room>> rooms_;
};
#endif  // FLOOR_H_

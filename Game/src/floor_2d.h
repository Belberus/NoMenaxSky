#ifndef FLOOR_2D_H_
#define FLOOR_2D_H_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <engine/core/scene.h>
#include <engine/events/collision.h>
#include <entityx/entityx.h>

#include "floor.h"

class Floor2D : public Floor {
 public:
  Floor2D(Game *parent_scene);
  void Update(entityx::TimeDelta dt) override;
  void OnPlayerEnteringDoor(Door entering_door) override;
  void OnPlayerEnteringBossDoorWithKey(BossDoor entering_door) override;
  void OnPlayerEnteringBossDoorWithoutKey() override;
  friend class FloorFactory;
};
#endif  // FLOOR_2D_H_
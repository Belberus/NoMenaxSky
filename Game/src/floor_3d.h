#ifndef FLOOR_3D_H_
#define FLOOR_3D_H_

#include "floor.h"

class Floor3D : public Floor {
 public:
  Floor3D(Game *parent_scene);
  void Update(entityx::TimeDelta dt) override;
  void OnPlayerEnteringDoor(Door entering_door) override;
  void OnPlayerEnteringBossDoorWithKey(BossDoor entering_door) override;
  void OnPlayerEnteringBossDoorWithoutKey() override;
  void PauseGame(bool pause) override;
  bool GetPaused() override;
  friend class FloorFactory; 

 private:
 	bool paused;
};
#endif  // FLOOR_3D_H_

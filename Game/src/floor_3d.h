#ifndef FLOOR_3D_H_
#define FLOOR_3D_H_

#include "floor.h"

class Floor3D : public Floor {
 public:
  Floor3D();
  void Update(entityx::TimeDelta dt) override;
  void OnPlayerEnteringDoor(Door entering_door) override;
  friend class FloorFactory3D;
};
#endif  // FLOOR_3D_H_

#ifndef FLOOR_FACTORY_3D_H_
#define FLOOR_FACTORY_3D_H_

#include <memory>
#include <string>

#include "floor_3d.h"

class FloorFactory3D {
 public:
  static std::unique_ptr<Floor3D> MakeFloor1();
};

#endif  // FLOOR_FACTORY_3D_H_

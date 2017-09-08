#ifndef SYSTEMS_3D_H_
#define SYSTEMS_3D_H_

#include <entityx/entityx.h>

#include "components.h"

#include "events.h"

class CameraFollowPlayerSystem
    : public entityx::System<CameraFollowPlayerSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

#endif  // SYSTEMS_3D_H_
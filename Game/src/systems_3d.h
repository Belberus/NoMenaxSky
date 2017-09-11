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

 private:
  static const float kCameraSpeed;
};

class KnighAnimationSystem3D : public entityx::System<KnighAnimationSystem3D> {
 public:
  KnighAnimationSystem3D();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  enum class State {
    kNull,
    kMoving,
    kAttacking,
    kShieldUp,
    kShieldDown,
    kMovingAttacking,
    kMovingDefending,
    kIdle
  };
  State current_state_;
};

class WizardAnimationSystem3D
    : public entityx::System<WizardAnimationSystem3D> {
 public:
  WizardAnimationSystem3D();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  enum class State { kNull, kMoving, kAttacking, kIdle };
  State current_state_;
};
#endif  // SYSTEMS_3D_H_
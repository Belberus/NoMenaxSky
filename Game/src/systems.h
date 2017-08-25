#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#include <unordered_map>

#include <engine/events/collision.h>
#include <engine/events/key_pressed.h>
#include <engine/events/key_released.h>

#include "components.h"

class KnightAnimationSystem : public entityx::System<KnightAnimationSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
 private:
  std::string lastAnim;
  float timer;
  float timer2;
};

class KnightAttackSystem : public entityx::System<KnightAttackSystem>,
                           public entityx::Receiver<KnightAttackSystem> {
 public:
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class KnightWalkingSystem : public entityx::System<KnightWalkingSystem> {
 public:
 	void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
}; 

class TurretWalkingSystem : public entityx::System<TurretWalkingSystem> {
 public:
 	void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class TurretIaSystem : public entityx::System<TurretIaSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
 private:
 	static const float turretSpeed;
};

class HealthSystem : public entityx::System<HealthSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class GhostAnimationSystem : public entityx::System<GhostAnimationSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class GhostIaSystem : public entityx::System<GhostIaSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  static const float kSpeed;
};

class MenuInputSystem : public entityx::System<MenuInputSystem>,
                        public entityx::Receiver<MenuInputSystem> {
 public:
  MenuInputSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
  void receive(const engine::events::KeyPressed &key_pressed);
  void receive(const engine::events::KeyReleased &key_released);

 private:
  bool up_pressed_;
  bool down_pressed_;
  bool enter_pressed_;
};

class OptionsInputSystem : public entityx::System<OptionsInputSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class PlayerInputSystem : public entityx::System<PlayerInputSystem>,
                          public entityx::Receiver<PlayerInputSystem> {
 public:
  PlayerInputSystem();
  void receive(const engine::events::KeyPressed &key_pressed);
  void receive(const engine::events::KeyReleased &key_released);

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  static const float kSpeed;
  static const float kAttackDuration;
  float time_passed_since_last_attack_;
  std::unordered_map<int, bool> keys_;
};

#endif  // SYSTEMS_H_

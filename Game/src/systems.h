#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#include <unordered_map>

#include <engine/events/collision.h>
#include <engine/events/ignore_collision.h>
#include <engine/events/key_pressed.h>
#include <engine/events/key_released.h>

#include "components.h"
/*
/// This systems is responbile of making sure that certain entities dont
/// collide.
class IgnoreCollisionSystem : public entityx::System<IgnoreCollisionSystem>,
                              public entityx::Receiver<IgnoreCollisionSystem> {
 public:
  IgnoreCollisionSystem(entityx::EntityManager *entity_manager,
                        entityx::EventManager *event_manager);
  void configure(entityx::EventManager &event_manager) override;
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
  /// When a component ghost is added, that means that a new ghost entity is
  /// being added, so we make sure that this entity doesnt collide with already
  /// existing low profile buildings.
  /// @param new_entity info about the event.
  void receive(const entityx::ComponentAddedEvent<Ghost> &new_entity);

  void receive(const entityx::ComponentAddedEvent<TurretProjectile> &new_entity);

 private:
  entityx::EntityManager *entity_manager_;
  entityx::EventManager *event_manager_;
  std::vector<entityx::Entity> ghosts_;
};
*/
class KnightAnimationSystem : public entityx::System<KnightAnimationSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  std::string lastAnim;
  std::string lastOrientation;
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

class ManuelethAnimationSystem : public entityx::System<ManuelethAnimationSystem> {
  public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class ManuelethIaSystem : public entityx::System<ManuelethIaSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class ManuelethAttackSystem : public entityx::System<ManuelethAttackSystem> {
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
};

class TrapIaSystem : public entityx::System<TrapIaSystem> {
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

class TurretAttackSystem : public entityx::System<TurretAttackSystem>,
                           public entityx::Receiver<TurretAttackSystem> {
 public:
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class EnemyProjectileAnimationSystem
    : public entityx::System<EnemyProjectile> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
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

class OptionsInputSystem : public entityx::System<OptionsInputSystem>, 
                           public entityx::Receiver<OptionsInputSystem>{
 public:
  OptionsInputSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

  void receive(const engine::events::KeyPressed &key_pressed);
  void receive(const engine::events::KeyReleased &key_released);

 private:
  bool options_up_pressed_;
  bool options_down_pressed_;
  bool options_enter_pressed_;
  bool options_right_pressed_;
  bool options_left_pressed_;
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

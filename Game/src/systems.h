#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#include <unordered_map>

#include <engine/events/collision.h>
#include <engine/events/ignore_collision.h>
#include <engine/events/key_pressed.h>
#include <engine/events/key_released.h>

#include "components.h"

#include "events.h"

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

class WizardAnimationSystem : public entityx::System<WizardAnimationSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  std::string lastAnim;
  std::string lastOrientation;
  float timer;
  float timer2;
};

class WizardAttackSystem : public entityx::System<WizardAttackSystem>,
                           public entityx::Receiver<WizardAttackSystem> {
 public:
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class TurretWalkingSystem : public entityx::System<TurretWalkingSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class LancerWalkingSystem : public entityx::System<LancerWalkingSystem> {
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

class ChestSystem : public entityx::System<ChestSystem>,
                           public entityx::Receiver<ChestSystem> {
public:
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
private:
  entityx::EventManager *event_manager_;
};

class LeverSystem : public entityx::System<LeverSystem>,
                           public entityx::Receiver<LeverSystem> {
public:
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
private:
  entityx::EventManager *event_manager_;
};

class ShieldSystem : public entityx::System<ShieldSystem>,
                           public entityx::Receiver<ShieldSystem> {
public:
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
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

class PauseInputSystem : public entityx::System<PauseInputSystem>,
                        public entityx::Receiver<PauseInputSystem> {
 public:
  PauseInputSystem();
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

  int mode;
  int music;
  int fx;
};

class SelectionInputSystem : public entityx::System<OptionsInputSystem>, 
                             public entityx::Receiver<OptionsInputSystem>{

  public:
    SelectionInputSystem();
    void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

    void receive(const engine::events::KeyPressed &key_pressed);
    void receive(const engine::events::KeyReleased &key_released);

  private:
    bool selection_right_pressed_;
    bool selection_left_pressed_;
    bool selection_enter_pressed_;


};

class DeathInputSystem :  public entityx::System<DeathInputSystem>,
                          public entityx::Receiver<DeathInputSystem> {

  public:
    DeathInputSystem();
    void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

    void receive(const engine::events::KeyPressed &key_pressed);
    void receive(const engine::events::KeyReleased &key_released);

  private:
    bool selection_enter_pressed_;
};

class TextInputSystem :  public entityx::System<TextInputSystem>,
                          public entityx::Receiver<TextInputSystem> {

  public:
    TextInputSystem();
    void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

    void receive(const engine::events::KeyPressed &key_pressed);
    void receive(const engine::events::KeyReleased &key_released);

  private:
    bool selection_enter_pressed_;
};

class PlayerInputSystem : public entityx::System<PlayerInputSystem>,
                          public entityx::Receiver<PlayerInputSystem> {
 public:
  PlayerInputSystem();
  void receive(const engine::events::KeyPressed &key_pressed);
  void receive(const engine::events::KeyReleased &key_released);
  void receive(const BackToGame &resumeGame);

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

  bool is_paused();

  void set_paused(bool paused);

 private:
  static const float kKnightSpeed;
  static const float kWizardSpeed;
  static const float kAttackDuration;
  static const float kAltAttackDuration;
  static const float kMagicAttackDuration;
  float time_passed_since_last_attack_;
  float time_passed_since_last_alt_attack_;
  float time_passed_since_last_magic_attack_;
  float knight_speed;
  float wizard_speed;
  std::unordered_map<int, bool> keys_;
  bool paused_;
};

class LancerIaSystem : public entityx::System<LancerIaSystem> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  static const float lancerSpeed;
};

class LancerAnimationSystem : public entityx::System<LancerAnimationSystem> {
  public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class LancerAttackSystem : public entityx::System<LancerAttackSystem>,
                           public entityx::Receiver<LancerAttackSystem> {
 public:
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class GhostAttackSystem : public entityx::System<GhostAttackSystem>,
                           public entityx::Receiver<GhostAttackSystem> {
 public:
  void configure(entityx::EventManager &event_manager) override;
  void receive(const engine::events::Collision &collision);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};




#endif  // SYSTEMS_H_

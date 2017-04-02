#pragma once
#include "Components.h"
#include "Messages.h"
#include "Shaders.h"
#include "Window.h"

glm::vec2 decompose(const glm::vec2 &v);

class KnightAnimationSystem : public entityx::System<KnightAnimationSystem> {
  bool getNext(entityx::ComponentHandle<KnightAnimation> knightAnimation,
               entityx::ComponentHandle<Graphics> graphics,
               entityx::TimeDelta dt, std::shared_ptr<AnimationClip> whatClip);

public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class GhostAnimationSystem : public entityx::System<GhostAnimationSystem> {
  bool getNext(entityx::ComponentHandle<GhostAnimation> ghostAnimation,
               entityx::ComponentHandle<Graphics> graphics,
               entityx::TimeDelta dt, std::shared_ptr<AnimationClip> whatClip);

public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class CollisionSystem : public entityx::System<CollisionSystem> {
  bool areColliding(Body const &body1, Body const &body2);

  glm::vec2 depthOfCollision(Body const &body1, Body const &body2);

  void resolveCollision(Body &body, Position &pos, glm::vec2 const &depth);

public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class GraphicsSystem : public entityx::System<GraphicsSystem> {
  Shaders &shaders;

public:
  GraphicsSystem(Shaders &shaders);
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class PlayerInputSystem : public entityx::System<PlayerInputSystem> {
  Window &window;

public:
  PlayerInputSystem(Window &window);

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
  
};

class PhysicsSystem : public entityx::System<PhysicsSystem> {
public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class HealthSystem : public entityx::System<HealthSystem> {
public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class DeathListener : public entityx::System<DeathListener>,
                      public entityx::Receiver<DeathMessage> {
private:
  // entityx::EntityManager &entityManager;
public:
  // void DeathListener(entityx::EntityManager
  // &entityManager):entityManager(entityManager){}
  void configure(entityx::EventManager &event_manager);

  void update(entityx::EntityManager &entities, entityx::EventManager &events,
              entityx::TimeDelta dt);

  void receive(const DeathMessage &deathMessage);
};

class KnightWeaponSystem : public entityx::System<KnightWeaponSystem> {
public:
  void update(entityx::EntityManager &entities, entityx::EventManager &events,
              entityx::TimeDelta dt);
};
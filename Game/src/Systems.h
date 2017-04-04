#pragma once
#include "Components.h"
#include "Messages.h"
#include "Shaders.h"
#include "Window.h"

glm::vec2 decompose(const glm::vec2 &v);

bool areColliding(Body const &body1, Body const &body2);

glm::vec2 depthOfCollision(Body const &body1, Body const &body2);

void resolveCollision(Body &body, Position &pos, glm::vec2 const &depth);

class KnightAnimationSystem : public entityx::System<KnightAnimationSystem> {
  bool getNext(entityx::ComponentHandle<KnightAnimation> knightAnimation,
               entityx::ComponentHandle<Graphics> graphics,
               entityx::TimeDelta dt, std::shared_ptr<AnimationClip> whatClip);

public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class MenuAnimationSystem : public entityx::System<MenuAnimationSystem> {
  bool getNext(entityx::ComponentHandle<MenuAnimation> menuAnimation,
               entityx::ComponentHandle<Graphics> graphics,
               entityx::TimeDelta dt, std::shared_ptr<AnimationClip> whatClip);

public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

class DoorSystem : public entityx::System<DoorSystem> {
	Window &window;
	
public:
	DoorSystem(Window &window);
	glm::vec2 centerOfTheBody(Body &body);
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
  
public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

  
};

class CollisionGhostSystem : public entityx::System<CollisionGhostSystem> {
  
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

class MenuInputSystem : public entityx::System<MenuInputSystem> {
  Window &window;

public:
  MenuInputSystem(Window &window);

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
  
};

class OptionsInputSystem : public entityx::System<OptionsInputSystem> {
  Window &window;

public:
  OptionsInputSystem(Window &window);

  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
  
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

class AttackListener : public entityx::System<AttackListener>,
                        public entityx::Receiver<AttackMessage> {
private:
  // entityx::EntityManager &entityManager;
public:
  // void DeathListener(entityx::EntityManager
  // &entityManager):entityManager(entityManager){}
  void configure(entityx::EventManager &event_manager);

  void update(entityx::EntityManager &entities, entityx::EventManager &events,
              entityx::TimeDelta dt);

  void receive(const AttackMessage &attackMessage);
};

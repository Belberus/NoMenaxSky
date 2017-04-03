#include "Game.h"
#include "EntitiesCreator.h"
#include "Systems.h"

Game::Game(Window &window, Shaders &shaders) {
  events.subscribe<InitRoomMessage>(*this);
  systems.add<KnightAnimationSystem>();
  // systems.add<GhostAnimationSystem>();
  systems.add<PlayerInputSystem>(window);
  systems.add<MenuInputSystem>(window);
  systems.add<PhysicsSystem>();
  systems.add<CollisionSystem>();
  systems.add<MenuAnimationSystem>();
  systems.add<GraphicsSystem>(shaders);
  systems.configure();
  EntitiesCreator::addEntityMenu(entities);
}

void Game::update(entityx::TimeDelta dt) {
  systems.update<PlayerInputSystem>(dt);
  systems.update<MenuInputSystem>(dt);
  systems.update<KnightAnimationSystem>(dt);
  // systems.update<GhostAnimationSystem>(dt);
  systems.update<GraphicsSystem>(dt);
  systems.update<PhysicsSystem>(dt);
  systems.update<CollisionSystem>(dt);
  systems.update<MenuAnimationSystem>(dt);
  systems.update<GraphicsSystem>(dt);
}

void Game::receive(const InitRoomMessage &initRoom) {
  entities.reset();
  EntitiesCreator::addEntityRoom1(entities);
}

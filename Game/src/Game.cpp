#include "Game.h"
#include "EntitiesCreator.h"
#include "Systems.h"
#include <irrKlang.h>
#include <stdio.h>

using namespace irrklang;
using namespace std;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

ISoundEngine* eng = createIrrKlangDevice();

Game::Game(Window &window, Shaders &shaders) {
  eng->setSoundVolume(0.25);
  eng->play2D("assets/media/intro.wav",true);
  events.subscribe<InitRoomMessage>(*this);
  events.subscribe<GoToRoomMessage>(*this);
  systems.add<KnightAnimationSystem>();
  systems.add<DoorSystem>(window);
  systems.add<GhostAnimationSystem>();
  systems.add<PlayerInputSystem>(window);
  systems.add<MenuInputSystem>(window);
  systems.add<PhysicsSystem>();
  systems.add<CollisionSystem>();
  systems.add<CollisionGhostSystem>();
  systems.add<MenuAnimationSystem>();
  systems.add<GraphicsSystem>(shaders);
  systems.configure();
  EntitiesCreator::addEntityMenu(entities);
}

void Game::update(entityx::TimeDelta dt) {
  systems.update<PlayerInputSystem>(dt);
  systems.update<MenuInputSystem>(dt);
  systems.update<MenuAnimationSystem>(dt);
  systems.update<KnightAnimationSystem>(dt);
  systems.update<GhostAnimationSystem>(dt);
  systems.update<DoorSystem>(dt);
  systems.update<GraphicsSystem>(dt);
  systems.update<PhysicsSystem>(dt);
  systems.update<CollisionSystem>(dt);
  systems.update<CollisionGhostSystem>(dt);
}

void Game::receive(const InitRoomMessage &initRoom) {
  entities.reset();
  eng->stopAllSounds();
  eng->play2D("assets/media/tune2.wav",true);
  EntitiesCreator::addEntityStarterRoom1(entities);
}

void Game::receive(const GoToRoomMessage &goToRoom) {
	std::cout<<"Estoy en la receive"<< std::endl;
  entities.reset();
  eng->stopAllSounds();
  eng->play2D("assets/media/tune2.wav",true);
  if (goToRoom.room == 2) {
	EntitiesCreator::addEntityRoom1(entities);
  }
}


#pragma once
#include "Shaders.h"
#include "Messages.h"
#include "Window.h"
#include <entityx/entityx.h>

class Game : entityx::EntityX, public entityx::Receiver<Game> {
public:
  Game(Window &window, Shaders &shaders);
  void update(entityx::TimeDelta dt);
  void receive(const InitRoomMessage &quitGame);
};

#pragma once
#include "Shaders.h"
#include "Window.h"
#include <entityx/entityx.h>
#include <glm/glm.hpp>

class Room : public entityx::EntityX {
  void addEntityKnight(entityx::EntityManager &entities);

  void addEntityRoom(entityx::EntityManager &entities);

  void addEntityGhost(entityx::EntityManager &entities, glm::vec3 position,
                      glm::vec2 body);

public:
  Room(Window &window, Shaders &shaders);

  void update(entityx::TimeDelta dt);
};

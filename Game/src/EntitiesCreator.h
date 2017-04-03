#pragma once
#include "Shaders.h"
#include "Window.h"
#include <entityx/Entity.h>
#include <glm/glm.hpp>

class EntitiesCreator {
public:
  static void addEntityKnight(entityx::EntityManager &entities);

  static void addEntityDeep(entityx::EntityManager &entities,
                            glm::vec3 position);

  static void addEntityWalls(entityx::EntityManager &entities);

  static void addEntityRoom1(entityx::EntityManager &entities);

  static void addEntityGhost(entityx::EntityManager &entities,
                             glm::vec3 position);

  static void addEntityMenu(entityx::EntityManager &entities);
};

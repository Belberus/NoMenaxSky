#pragma once
#include "Shaders.h"
#include "Window.h"
#include <entityx/Entity.h>
#include <glm/glm.hpp>

class EntitiesCreator {
public:
  static void addEntityKnight(entityx::EntityManager &entities, glm::vec3 position);

  static void addEntityDeep(entityx::EntityManager &entities,
                            glm::vec3 position);

  static void addEntityWalls(entityx::EntityManager &entities);

  static void addEntityRoom1(entityx::EntityManager &entities, int fromWhere);

  static void addEntityGhost(entityx::EntityManager &entities,
                             glm::vec3 position);

  static void addEntityMenu(entityx::EntityManager &entities);

  static void addEntityOptions(entityx::EntityManager &entities);

  static void addEntityStarterRoom1(entityx::EntityManager &entities);

  static void addEntityWallsStarter(entityx::EntityManager &entities);

  static void addEntityDoorStarter(entityx::EntityManager &entities);
  static void addEntityDoor(entityx::EntityManager &entities, int room, int place);

  static void addEntityBossEntrance1(entityx::EntityManager &entities);
  static void addEntityWallsBossEntrance1(entityx::EntityManager &entities);
};

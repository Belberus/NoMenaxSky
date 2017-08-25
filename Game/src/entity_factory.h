#ifndef ENTITY_FACTORY_H_
#define ENTITY_FACTORY_H_
#include <vector>

#include <entityx/Entity.h>
#include <glm/glm.hpp>

class EntityFactory {
 public:
  static std::vector<entityx::Entity> MakeKnight(
      entityx::EntityManager &entities, const glm::vec3 &position);

  static std::vector<entityx::Entity> MakeGhost(
      entityx::EntityManager &entities, const glm::vec3 &position);

  static std::vector<entityx::Entity> MakeTurret(
      entityx::EntityManager &entities, const glm::vec3 &position);
};
#endif  // ENTITY_FACTORY_H_
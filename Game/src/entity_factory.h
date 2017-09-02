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
      entityx::EntityManager &entities, const glm::vec3 &position, const float frecuencia);

  static std::vector<entityx::Entity> MakeTrap(
      entityx::EntityManager &entities, const glm::vec3 &position,  const std::string &orient, const float frecuencia);

  static std::vector<entityx::Entity> MakeManueleth(
      entityx::EntityManager &entities, const glm::vec3 &position);

  static std::vector<entityx::Entity> MakeLancer(
      entityx::EntityManager &entities, const glm::vec3 &position);

  static std::vector<entityx::Entity> MakeEnemyProjectile(
      entityx::EntityManager &entities, const glm::vec3 &position, const float &rotation, const glm::vec3 &velocity, const std::string &type);
};
#endif  // ENTITY_FACTORY_H_
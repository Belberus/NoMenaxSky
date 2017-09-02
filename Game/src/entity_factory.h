#ifndef ENTITY_FACTORY_H_
#define ENTITY_FACTORY_H_
#include <vector>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

struct EntityFactory {
  virtual std::vector<entityx::Entity> MakeKnight(
      entityx::EntityManager &entities, const glm::vec3 &position) = 0;

  virtual std::vector<entityx::Entity> MakeGhost(
      entityx::EntityManager &entities, const glm::vec3 &position) = 0;

  virtual std::vector<entityx::Entity> MakeTurret(
      entityx::EntityManager &entities, const glm::vec3 &position,
      const float frecuencia) = 0;

  virtual std::vector<entityx::Entity> MakeTrap(
      entityx::EntityManager &entities, const glm::vec3 &position,
      const std::string &orient, const float frecuencia) = 0;

  virtual std::vector<entityx::Entity> MakeManueleth(
      entityx::EntityManager &entities, const glm::vec3 &position) = 0;

  virtual std::vector<entityx::Entity> MakeLancer(
      entityx::EntityManager &entities, const glm::vec3 &position) = 0;

  virtual std::vector<entityx::Entity> MakeEnemyProjectile(
      entityx::EntityManager &entities, const glm::vec3 &position,
      const float &rotation, const glm::vec3 &velocity,
      const std::string &type) = 0;
};
#endif  // ENTITY_FACTORY_H_
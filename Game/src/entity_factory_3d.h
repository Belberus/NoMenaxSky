#ifndef ENTITY_FACTORY_3D_H_
#define ENTITY_FACTORY_3D_H_

#include "entity_factory.h"

struct EntityFactory3D : public EntityFactory {
  std::vector<entityx::Entity> MakeKnight(entityx::EntityManager &entities,
                                          const glm::vec3 &position) override;

  std::vector<entityx::Entity> MakeGhost(entityx::EntityManager &entities,
                                         const glm::vec3 &position) override;

  std::vector<entityx::Entity> MakeTurret(entityx::EntityManager &entities,
                                          const glm::vec3 &position,
                                          const float frecuencia) override;

  std::vector<entityx::Entity> MakeTrap(entityx::EntityManager &entities,
                                        const glm::vec3 &position,
                                        const std::string &orient,
                                        const float frecuencia) override;

  std::vector<entityx::Entity> MakeManueleth(
      entityx::EntityManager &entities, const glm::vec3 &position) override;

  std::vector<entityx::Entity> MakeEnemyProjectile(
      entityx::EntityManager &entities, const glm::vec3 &position,
      const float &rotation, const glm::vec3 &velocity,
      const std::string &type) override;

  std::vector<entityx::Entity> MakeLancer(entityx::EntityManager &entities,
                                          const glm::vec3 &position) override;
};
#endif  // ENTITY_FACTORY_3D_H_

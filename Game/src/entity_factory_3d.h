#ifndef ENTITY_FACTORY_3D_H_
#define ENTITY_FACTORY_3D_H_

#include <vector>

#include <entityx/Entity.h>
#include <glm/glm.hpp>

// TODO: put this in a namespace
// TODO: make an interface for the 2d and 3d entity factory
class EntityFactory3D {
 public:
  ;
  static std::vector<entityx::Entity> MakeKnight(
      entityx::EntityManager &entities, const glm::vec3 &position);
};
#endif  // ENTITY_FACTORY_3D_H_
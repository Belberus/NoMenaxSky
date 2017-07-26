#ifndef ENGINE_EVENTS_COLLISION_H_
#define ENGINE_EVENTS_COLLISION_H_
#include <entityx/entityx.h>
namespace engine {
namespace events {
struct Collision {
  entityx::Entity e0;
  entityx::Entity e1;
  Collision(entityx::Entity e0, entityx::Entity e1) : e0(e0), e1(e1) {}
};
}  // namespace events
}  // namespace engine
#endif  // ENGINE_EVENTS_COLLISION_H_

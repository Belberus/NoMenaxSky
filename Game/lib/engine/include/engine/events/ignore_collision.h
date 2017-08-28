#ifndef ENGINE_EVENTS_IGNORE_COLLISION_H_
#define ENGINE_EVENTS_IGNORE_COLLISION_H_
#include <entityx/entityx.h>
namespace engine {
namespace events {
/// Send this event if you want the physics system to ignore collisions between
/// two given entitites.
struct IgnoreCollision {
  entityx::Entity e0;
  entityx::Entity e1;
  bool ignore;
  /// Constructs a new ignore collision event.
  /// @param e0 first entity.
  /// @param e1 second entity.
  /// @param ignore if true collisions between e0 and e1 will be ignored, if
  /// false and if e0 and e1 were previously ignored then, e0 and e1 can collide
  /// again; if they werent ignored previously then nothing happens
  IgnoreCollision(entityx::Entity e0, entityx::Entity e1, bool ignore)
      : e0(e0), e1(e1), ignore(ignore) {}
};
}  // namespace events
}  // namespace engine
#endif  // ENGINE_EVENTS_IGNORE_COLLISION_H_

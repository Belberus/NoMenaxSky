#ifndef ENGINE_SYSTEMS_TWO_D_COLLIDER_RENDERER_H_
#define ENGINE_SYSTEMS_TWO_D_COLLIDER_RENDERER_H_
#include <entityx/entityx.h>

namespace engine {
namespace systems {
namespace two_d {
class ColliderRenderer : public entityx::System<ColliderRenderer> {
 public:
  ColliderRenderer();
  ~ColliderRenderer();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};
}  // namespace two_d
}  // namespace systems
}  // namespace engine
#endif  // ENGINE_SYSTEMS_TWO_D_COLLIDER_RENDERER_H_

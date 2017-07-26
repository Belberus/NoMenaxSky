#ifndef ENGINE_SYSTEMS_TWO_D_SPRITE_ANIMATOR_H_
#define ENGINE_SYSTEMS_TWO_D_SPRITE_ANIMATOR_H_
#include <entityx/entityx.h>

namespace engine {
namespace systems {
namespace two_d {
class SpriteAnimator : public entityx::System<SpriteAnimator> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};
}  // namespace two_d
}  // namespace systems
}  // namespace engine
#endif  // ENGINE_SYSTEMS_TWO_D_SPRITE_ANIMATOR_H_

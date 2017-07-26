#ifndef ENGINE_SYSTEMS_TWO_D_COLOR_ANIMATOR_H_
#define ENGINE_SYSTEMS_TWO_D_COLOR_ANIMATOR_H_
#include <entityx/entityx.h>
namespace engine {
namespace systems {
namespace two_d {
class ColorAnimator : public entityx::System<ColorAnimator> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
};
}  // namespace two_d
}  // namespace systems
}  // namespace engine
#endif  // ENGINE_SYSTEMS_TWO_D_COLOR_ANIMATOR_H_
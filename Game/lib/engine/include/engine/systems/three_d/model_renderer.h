#ifndef ENGINE_SYSTEMS_THREE_D_MODEL_RENDERER_H_
#define ENGINE_SYSTEMS_THREE_D_MODEL_RENDERER_H_
#include <entityx/entityx.h>
namespace engine {
namespace systems {
namespace three_d {
/// This system is responsible of rendering a 3D model.
class ModelRenderer : public entityx::System<ModelRenderer> {
 public:
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
};
}  // namespace three_d
}  // namespace systems
}  // namespace engine
#endif  // ENGINE_SYSTEMS_THREE_D_MODEL_RENDERER_H_
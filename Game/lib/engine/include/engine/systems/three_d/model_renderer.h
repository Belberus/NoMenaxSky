#ifndef ENGINE_SYSTEMS_THREE_D_MODEL_RENDERER_H_
#define ENGINE_SYSTEMS_THREE_D_MODEL_RENDERER_H_

#include <memory>

#include <entityx/entityx.h>

namespace engine {
namespace systems {
namespace three_d {
/// This system is responsible of rendering a 3D model.
class ModelRenderer : public entityx::System<ModelRenderer> {
 public:
  ModelRenderer();
  ModelRenderer(const ModelRenderer &renderer);
  ModelRenderer &operator=(const ModelRenderer &renderer);
  ~ModelRenderer();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};
}  // namespace three_d
}  // namespace systems
}  // namespace engine
#endif  // ENGINE_SYSTEMS_THREE_D_MODEL_RENDERER_H_
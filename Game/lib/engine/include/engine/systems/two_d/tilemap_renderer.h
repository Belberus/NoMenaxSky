#ifndef ENGINE_SYSTEMS_TWO_D_TILEMAP_RENDERER_H_
#define ENGINE_SYSTEMS_TWO_D_TILEMAP_RENDERER_H_
#include <entityx/entityx.h>

namespace engine {
namespace systems {
namespace two_d {
class TilemapRenderer : public entityx::System<TilemapRenderer> {
 public:
  TilemapRenderer();
  ~TilemapRenderer();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};
}  // namespace two_d
}  // namespace systems
}  // namespace engine
#endif  // ENGINE_SYSTEMS_TWO_D_TILEMAP_RENDERER_H_

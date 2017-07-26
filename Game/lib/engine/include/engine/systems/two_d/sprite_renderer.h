#ifndef ENGINE_SYSTEMS_TWO_D_SPRITE_RENDERER_H_
#define ENGINE_SYSTEMS_TWO_D_SPRITE_RENDERER_H_

#include <vector>

#include <entityx/entityx.h>

#include "engine/components/two_d/texture.h"
#include "engine/utils/rectangle.h"

namespace engine {
namespace systems {
namespace two_d {
class SpriteRenderer : public entityx::System<SpriteRenderer> {
 public:
  SpriteRenderer();
  ~SpriteRenderer();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

 private:
  static std::vector<glm::vec2> GetTexCoordVertices(
      const engine::utils::Rectangle &rect);
  static void NormalizeTexCoordVertices(
      std::vector<glm::vec2> &coord,
      const engine::components::two_d::Texture &tex);
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};
}  // namespace two_d
}  // namespace systems
}  // namespace engine

#endif  // ENGINE_SYSTEMS_TWO_D_SPRITE_RENDERER_H_

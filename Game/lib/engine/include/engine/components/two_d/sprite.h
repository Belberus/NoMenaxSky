#ifndef ENGINE_COMPONENTS_TWO_D_SPRITE_H_
#define ENGINE_COMPONENTS_TWO_D_SPRITE_H_

#include <memory>

#include "engine/components/two_d/texture.h"
#include "engine/utils/rectangle.h"

namespace engine {
namespace components {
namespace two_d {
class Sprite {
 public:
  Sprite(const std::shared_ptr<Texture> &texture)
      : texture_(texture),
        texture_coordinates_(glm::vec2(0, 0), glm::vec2(texture->GetWidth(),
                                                        texture->GetHeight())),
        color_(0.0f, 0.0f, 0.0f, 0.0f) {}
  Sprite(const std::shared_ptr<Texture> &texture,
         const engine::utils::Rectangle &rect)
      : texture_(texture),
        texture_coordinates_(rect),
        color_(0.0f, 0.0f, 0.0f, 0.0f) {}
  std::shared_ptr<Texture> GetTexture() { return texture_; }
  engine::utils::Rectangle GetTextureCoord() { return texture_coordinates_; }
  void SetTexture(const std::shared_ptr<Texture> &tex) { texture_ = tex; }
  void SetTextureCoord(const engine::utils::Rectangle &tex_coord) {
    texture_coordinates_ = tex_coord;
  }
  void SetColor(const glm::vec4 &color) { color_ = color; }
  glm::vec4 GetColor() { return color_; };

 private:
  std::shared_ptr<Texture> texture_;
  engine::utils::Rectangle texture_coordinates_;
  glm::vec4 color_;
};
}  // namespace two_d
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_TWO_D_SPRITE_H_

#ifndef ENGINE_COMPONENTS_TWO_D_TEXTURE_H_
#define ENGINE_COMPONENTS_TWO_D_TEXTURE_H_

#include <memory>
#include <string>

namespace engine {
namespace systems {
namespace two_d {
class SpriteRenderer;
class TilemapRenderer;
}  // namespace two_d
}  // namespace systems
}  // namespace engine

namespace engine {
namespace components {
namespace two_d {
class Texture {
 public:
  Texture(const std::string &path);
  Texture(const Texture &tex) = delete;
  Texture &operator=(const Texture &tex) = delete;
  ~Texture();
  int GetWidth() const;
  int GetHeight() const;

 private:
  int width_;
  int height_;
  int ch_;
  std::unique_ptr<unsigned char, void (*)(unsigned char *)> img_data_;
  unsigned int texture_id_;
  unsigned int texture_unit_id_;
  static int next_texture_unit_;
  friend class engine::systems::two_d::SpriteRenderer;
  friend class engine::systems::two_d::TilemapRenderer;
};
}  // namespace two_d
}  // namespace components
}  // namespace engine

#endif  // ENGINE_COMPONENTS_TWO_D_TEXTURE_H_

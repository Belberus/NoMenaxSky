#ifndef ENGINE_UTILS_RECTANGLE_H_
#define ENGINE_UTILS_RECTANGLE_H_
#include <glm/glm.hpp>
namespace engine {
namespace utils {
class Rectangle {
 public:
  Rectangle(const glm::vec2& position, const glm::vec2& size)
      : position_(position), size_(size) {}
  glm::vec2 GetPosition() const { return position_; }
  glm::vec2 GetSize() const { return size_; }
  void SetPosition(const glm::vec2& pos) { position_ = pos; }
  void SetSize(const glm::vec2& size) { size_ = size; }

 private:
  glm::vec2 position_;
  glm::vec2 size_;
};
}  // namespace utils
}  // namespace engine
#endif  // ENGINE_UTILS_RECTANGLE_H_
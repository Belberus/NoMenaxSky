#ifndef ENGINE_COMPONENTS_TWO_D_COLOR_ANIMATION_H_
#define ENGINE_COMPONENTS_TWO_D_COLOR_ANIMATION_H_

#include <vector>

#include <glm/glm.hpp>

namespace engine {
namespace systems {
namespace two_d {
class ColorAnimator;
}  // namespace two_d
}  // namespace systems
}  // namespace engine

namespace engine {
namespace components {
namespace two_d {
class ColorAnimation {
 public:
  struct KeyFrame {
    KeyFrame(const glm::vec3 &color, const float &duration)
        : color(color), duration(duration) {}
    glm::vec3 color;
    float duration;
  };

  ColorAnimation(const std::vector<KeyFrame> &frames)
      : time_passed_(0.0f),
        play_(false),
        current_key_frame_(1),
        key_frames_(frames) {
    key_frames_.insert(key_frames_.begin(),
                       KeyFrame(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f));
  };
  void Play() { play_ = true; }

 private:
  float time_passed_;
  bool play_;
  int current_key_frame_;
  std::vector<KeyFrame> key_frames_;

  friend class engine::systems::two_d::ColorAnimator;
};
}  // namespace two_d
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_TWO_D_COLOR_ANIMATION_H_
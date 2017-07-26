#ifndef ENGINE_COMPONENTS_TWO_D_ANIMATION_H_
#define ENGINE_COMPONENTS_TWO_D_ANIMATION_H_

#include <string>
#include <unordered_map>
#include <vector>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

#include "engine/components/two_d/sprite.h"
#include "engine/components/two_d/texture.h"
#include "engine/utils/rectangle.h"

namespace engine {
namespace systems {
namespace two_d {
class SpriteAnimator;
}  // namespace two_d
}  // namespace systems
}  // namespace engine

namespace engine {
namespace components {
namespace two_d {
class SpriteAnimation {
 public:
  class AnimationClip {
   public:
    AnimationClip(const std::string& name,
                  const std::shared_ptr<Texture>& texture,
                  std::vector<engine::utils::Rectangle>& frames,
                  entityx::TimeDelta time_per_frame);
    engine::utils::Rectangle operator[](std::size_t anim);
    std::string GetName() const;
    entityx::TimeDelta GetTimePerFrame() const;
    std::size_t GetNumOfFrames() const;
    std::shared_ptr<Texture> GetTexture();

   private:
    std::string name_;
    std::vector<engine::utils::Rectangle> clip_;
    std::shared_ptr<Texture> texture_;
    entityx::TimeDelta time_per_frame_;
  };
  SpriteAnimation(const std::vector<AnimationClip>& clips);
  void AddAnimationClip(const AnimationClip& clip);
  AnimationClip operator[](const std::string& anim);
  void Play(const std::string& anim_to_play);
  std::string GetClipBeingPlayed();

 private:
  std::unordered_map<std::string, AnimationClip> clips_;
  std::string playing_clip_;

  // variables used only by the animation system
  friend class engine::systems::two_d::SpriteAnimator;
  entityx::TimeDelta time_passed_;
  std::string previous_clip_;
  int playing_frame_;
};
}  // namespace two_d
}  // namespace components
}  // namespace engine

#endif  // ENGINE_COMPONENTS_TWO_D_ANIMATION_H_

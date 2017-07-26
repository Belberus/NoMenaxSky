#include "engine/systems/two_d/color_animator.h"

#include <glm/glm.hpp>

#include "engine/components/two_d/color_animation.h"
#include "engine/components/two_d/sprite.h"

// HACK: for now it should be used with only one keyframe
void engine::systems::two_d::ColorAnimator::update(
    entityx::EntityManager& es, entityx::EventManager& events,
    entityx::TimeDelta dt) {
  es.each<engine::components::two_d::Sprite,
          engine::components::two_d::ColorAnimation>(
      [&](entityx::Entity entity, engine::components::two_d::Sprite& sprite,
          engine::components::two_d::ColorAnimation& color_animation) {
        if (color_animation.play_) {
          float complete_percentage = 0.0f;
          auto duration =
              color_animation.key_frames_[color_animation.current_key_frame_]
                  .duration;
          if ((color_animation.time_passed_ += dt) > duration) {
            color_animation.time_passed_ -= duration;
            ++color_animation.current_key_frame_;
          }

          int last_frame_mix;
          if (color_animation.current_key_frame_ >=
              color_animation.key_frames_.size()) {
            complete_percentage = 1.0f;
            last_frame_mix = color_animation.key_frames_.size() - 1;
            color_animation.play_ = false;
            color_animation.current_key_frame_ = 1;
            color_animation.time_passed_ = 0.0f;
          } else {
            complete_percentage =
                color_animation.time_passed_ /
                color_animation.key_frames_[color_animation.current_key_frame_]
                    .duration;
            last_frame_mix = color_animation.current_key_frame_;
          }
          sprite.SetColor(glm::vec4(
              glm::mix(color_animation.key_frames_[last_frame_mix - 1].color,
                       color_animation.key_frames_[last_frame_mix].color,
                       complete_percentage),
              0.0f));
        }
      });
}
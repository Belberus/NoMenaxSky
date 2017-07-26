#include "engine/systems/two_d/sprite_animator.h"

#include "engine/components/two_d/sprite_animation.h"

void engine::systems::two_d::SpriteAnimator::update(
    entityx::EntityManager &es, entityx::EventManager &events,
    entityx::TimeDelta dt) {
  entityx::ComponentHandle<engine::components::two_d::SpriteAnimation>
      animation;
  entityx::ComponentHandle<engine::components::two_d::Sprite> sprite;
  for (const auto &e : es.entities_with_components(animation, sprite)) {
    if (animation->previous_clip_ != animation->playing_clip_) {
      animation->playing_frame_ = 0;
      animation->time_passed_ = 0;
      animation->previous_clip_ = animation->playing_clip_;
    } else {
      animation->time_passed_ += dt;
    }
    auto playing_clip = (*animation)[animation->playing_clip_];
    if (animation->time_passed_ >= playing_clip.GetTimePerFrame() / 1000.0) {
      animation->time_passed_ = 0;
      animation->playing_frame_ =
          (animation->playing_frame_ + 1) % playing_clip.GetNumOfFrames();
    }
    sprite->SetTexture(playing_clip.GetTexture());
    sprite->SetTextureCoord(playing_clip[animation->playing_frame_]);
  }
}

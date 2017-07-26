#include "engine/components/two_d/sprite_animation.h"

using namespace engine::components::two_d;

SpriteAnimation::SpriteAnimation(const std::vector<AnimationClip>& clips)
    : clips_(), playing_clip_(clips[0].GetName()) {
  for (auto& c : clips) {
    clips_.insert(std::make_pair(c.GetName(), c));
  }
}

void SpriteAnimation::AddAnimationClip(const AnimationClip& clip) {
  clips_.emplace(clip.GetName(), clip);
}

SpriteAnimation::AnimationClip SpriteAnimation::operator[](
    const std::string& anim) {
  return clips_.at(anim);
}

void SpriteAnimation::Play(const std::string& anim_to_play) {
  playing_clip_ = anim_to_play;
}

std::string SpriteAnimation::GetClipBeingPlayed() { return playing_clip_; }

SpriteAnimation::AnimationClip::AnimationClip(
    const std::string& name, const std::shared_ptr<Texture>& texture,
    std::vector<engine::utils::Rectangle>& frames,
    entityx::TimeDelta time_per_frame)
    : name_(name),
      texture_(texture),
      clip_(frames),
      time_per_frame_(time_per_frame) {}

engine::utils::Rectangle SpriteAnimation::AnimationClip::operator[](
    std::size_t frame) {
  return clip_.at(frame);
}

std::string engine::components::two_d::SpriteAnimation::AnimationClip::GetName()
    const {
  return name_;
}

std::size_t SpriteAnimation::AnimationClip::GetNumOfFrames() const {
  return clip_.size();
}

std::shared_ptr<Texture>
engine::components::two_d::SpriteAnimation::AnimationClip::GetTexture() {
  return texture_;
}

entityx::TimeDelta SpriteAnimation::AnimationClip::GetTimePerFrame() const {
  return time_per_frame_;
}
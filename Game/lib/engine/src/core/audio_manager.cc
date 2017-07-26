#include "engine/core/audio_manager.h"

#include <iostream>

engine::core::AudioManager::AudioManager() {
  sound_engine_ = irrklang::createIrrKlangDevice();
  if (!sound_engine_) {
    std::cerr << "Error starting up the sound engine.\n";
  }
}

engine::core::AudioManager::~AudioManager() { sound_engine_->drop(); }

void engine::core::AudioManager::PlaySound(const std::string& sound_filename,
                                           bool looped) {
  sound_engine_->play2D(sound_filename.c_str(), looped);
}

void engine::core::AudioManager::StopAllSounds() {
  sound_engine_->stopAllSounds();
}

void engine::core::AudioManager::SetVolume(float volume) {
  sound_engine_->setSoundVolume(volume);
}

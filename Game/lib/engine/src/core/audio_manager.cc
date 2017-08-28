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
                                           bool looped, float volume) {
  irrklang::ISoundSource* sound = 
    sound_engine_->getSoundSource(sound_filename.c_str());
  sound->setDefaultVolume(volume);
  if(!sound_engine_->isCurrentlyPlaying(sound)){
    sound_engine_->play2D(sound, looped);
  }
  //else is already playing and will overlap
}

void engine::core::AudioManager::StopAllSounds() {
  sound_engine_->stopAllSounds();
}

void engine::core::AudioManager::SetVolume(float volume) {
  sound_engine_->setSoundVolume(volume);
}

void engine::core::AudioManager::PlaySound3D(const std::string& sound_filename,
                                           bool looped, glm::vec3 pos) {
	irrklang::vec3df position(pos.x,pos.y,pos.z);
  sound_engine_->play3D(sound_filename.c_str(), position, looped);
}

//This is useless but may come in handy
/*void engine::core::AudioManager::LoadSound(const std::string &sound_filename){
  irrklang::ISoundSource* newSound = sound_engine_->addSoundSourceFromFile(sound_filename.c_str());
}*/

void engine::core::AudioManager::ResetSounds(){
  sound_engine_->removeAllSoundSources();
}
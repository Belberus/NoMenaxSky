#include "engine/core/audio_manager.h"

#include <iostream>

engine::core::AudioManager::AudioManager() {
  sound_engine_fx_ = irrklang::createIrrKlangDevice();
  sound_engine_music_ = irrklang::createIrrKlangDevice();
  if (!sound_engine_fx_) {
    std::cerr << "Error starting up the fx sound engine.\n";
  }
  if (!sound_engine_music_) {
    std::cerr << "Error starting up the music sound engine.\n";
  }
}

engine::core::AudioManager::~AudioManager() { 
  sound_engine_fx_->drop(); 
  sound_engine_music_->drop();
}

void engine::core::AudioManager::PlaySound(const std::string& sound_filename,
                                           bool looped, float volume) {
  
  if(sound_filename.find("fx") != std::string::npos){
    irrklang::ISoundSource* sound = 
    sound_engine_fx_->getSoundSource(sound_filename.c_str());
      sound->setDefaultVolume(volume);
      if(!sound_engine_fx_->isCurrentlyPlaying(sound)){
        sound_engine_fx_->play2D(sound, looped);
      }
      //Else is already playing and will overlap
  }
  else if(sound_filename.find("music") != std::string::npos){
    irrklang::ISoundSource* sound = 
    sound_engine_music_->getSoundSource(sound_filename.c_str());
      sound->setDefaultVolume(volume);
      if(!sound_engine_music_->isCurrentlyPlaying(sound)){
        sound_engine_music_->play2D(sound, looped);
      }
      //Else is already playing and will overlap
  }
  //Else can't reach here unless you tried to play something
  //outside assets, in that case, fuck off
}

void engine::core::AudioManager::StopAllSounds() {
  sound_engine_fx_->stopAllSounds();
  sound_engine_music_->stopAllSounds();
}

void engine::core::AudioManager::StopMusic() {
  sound_engine_music_->stopAllSounds();
}

void engine::core::AudioManager::SetVolumeFX(float volume) {
  sound_engine_fx_->setSoundVolume(volume);
}

void engine::core::AudioManager::SetVolumeMusic(float volume) {
  sound_engine_music_->setSoundVolume(volume);
}

// ONLY FX PLAY IN 3D, NO NEED TO PLAY MUSIC IN 3D FFS
void engine::core::AudioManager::PlaySound3D(const std::string& sound_filename,
                                           bool looped, glm::vec3 pos, float volume) {
	irrklang::vec3df position(pos.x,pos.y,pos.z);
  irrklang::ISoundSource* sound = 
    sound_engine_fx_->getSoundSource(sound_filename.c_str());
  sound->setDefaultVolume(volume);
  if(!sound_engine_fx_->isCurrentlyPlaying(sound)){
    sound_engine_fx_->play3D(sound_filename.c_str(), position, looped);
  }
  //Else is already playing and will overlap
}

//This is useless but may come in handy
/*void engine::core::AudioManager::LoadSound(const std::string &sound_filename){
  irrklang::ISoundSource* newSound = sound_engine_->addSoundSourceFromFile(sound_filename.c_str());
}*/

void engine::core::AudioManager::ResetSounds(){
  sound_engine_fx_->removeAllSoundSources();
  sound_engine_music_->removeAllSoundSources();
}
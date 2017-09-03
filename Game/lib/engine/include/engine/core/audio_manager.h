#ifndef ENGINE_CORE_AUDIO_MANAGER_H_
#define ENGINE_CORE_AUDIO_MANAGER_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <irrKlang.h>

namespace engine {
namespace core {
class AudioManager {
 public:
  AudioManager();
  ~AudioManager();
  void PlaySound(const std::string &sound_filename, bool looped, float volume);
  void StopAllSounds();
  void StopMusic();
  void SetVolumeFX(float volume);
  void SetVolumeMusic(float volume);
  void PlaySound3D(const std::string &sound_filename, bool looped,
  						glm::vec3 pos, float volume);
  void LoadSound(const std::string &sound_filename);
  void ResetSounds();
  bool getMusicMute();
  bool getFxMute();

 private:
  irrklang::ISoundEngine *sound_engine_fx_;
  irrklang::ISoundEngine *sound_engine_music_;
  bool music_mute;
  bool fx_mute;
};
}  // namespace core
}  // namespace engine
#endif  // ENGINE_CORE_ADUIO_MANAGER_H_

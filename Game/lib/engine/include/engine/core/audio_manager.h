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
  void SetVolume(float volume);
  void PlaySound3D(const std::string &sound_filename, bool looped,
  						glm::vec3 pos);
  void LoadSound(const std::string &sound_filename);
  void ResetSounds();

 private:
  irrklang::ISoundEngine *sound_engine_;
  //std::vector<irrklang::ISoundSource*> sounds_;
};
}  // namespace core
}  // namespace engine
#endif  // ENGINE_CORE_ADUIO_MANAGER_H_

#ifndef ENGINE_CORE_AUDIO_MANAGER_H_
#define ENGINE_CORE_AUDIO_MANAGER_H_

#include <string>

#include <irrKlang.h>

namespace engine {
namespace core {
class AudioManager {
 public:
  AudioManager();
  ~AudioManager();
  void PlaySound(const std::string &sound_filename, bool looped);
  void StopAllSounds();
  void SetVolume(float volume);

 private:
  irrklang::ISoundEngine *sound_engine_;
};
}  // namespace core
}  // namespace engine
#endif  // ENGINE_CORE_ADUIO_MANAGER_H_

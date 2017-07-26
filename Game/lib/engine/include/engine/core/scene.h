#ifndef ENGINE_CORE_SCENE_H_
#define ENGINE_CORE_SCENE_H_

#include <entityx/entityx.h>

namespace engine {
namespace core {
class Scene : public entityx::EntityX {
 public:
  virtual void Update(entityx::TimeDelta dt) = 0;
};

}  // namespace core
}  // namespace engine
#endif  // ENGINE_CORE_SCENE_H_
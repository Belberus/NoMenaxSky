#ifndef PAUSE_MENU_H_
#define PAUSE_MENU_H_
#include <engine/core/scene.h>

#include "game.h"
class PauseMenu : public engine::core::Scene {
 public:
  PauseMenu(Game *parent_scene);
  void Update(entityx::TimeDelta dt);
};
#endif  // PAUSE_MENU_H_
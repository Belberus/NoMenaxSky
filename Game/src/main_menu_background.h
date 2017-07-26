#ifndef MAIN_MENU_BACKGROUND_H_
#define MAIN_MENU_BACKGROUND_H_

#include <entityx/entityx.h>

#include <engine/core/scene.h>

class MainMenuBackground : public engine::core::Scene {
 public:
  MainMenuBackground();
  void Update(entityx::TimeDelta dt) override;
};

#endif  // MAIN_MENU_BACKGROUND_H_

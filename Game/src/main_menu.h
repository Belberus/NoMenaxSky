#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

#include <engine/core/scene.h>
#include <entityx/entityx.h>

#include "events.h"

class MainMenu : public engine::core::Scene,
                 public entityx::Receiver<MainMenu> {
 public:
  MainMenu(engine::core::Scene *parent_scene);
  void Update(entityx::TimeDelta dt) override;
  void receive(const StartGame &event);

 private:
  void AddEntities();
  engine::core::Scene *parent_scene_;
};

#endif  // MAIN_MENU_H_

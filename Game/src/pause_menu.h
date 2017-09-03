#ifndef PAUSE_MENU_H_
#define PAUSE_MENU_H_
#include <engine/core/scene.h>
#include <entityx/entityx.h>

#include "game.h"
#include "events.h"
#include "components.h"

class PauseMenu : public engine::core::Scene,
				  public entityx::Receiver<PauseMenu>{
 public:
  PauseMenu(Game *parent_scene);
  void Update(entityx::TimeDelta dt) override;

  void receive(const BackToMainMenu &event);
  void receive(const BackToGame &event);
  void receive(const MuteMusic &muteMusic);
  void receive(const MuteFx &muteFx);
 private:
 	entityx::Entity fx_option_;
 	entityx::Entity music_option_;
 	Game *parent_scene_;
};
#endif  // PAUSE_MENU_H_
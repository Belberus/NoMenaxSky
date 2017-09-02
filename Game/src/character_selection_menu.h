#ifndef CHARACTER_SELECTION_MENU_H_
#define CHARACTER_SELECTION_MENU_H_

#include <engine/core/scene.h>
#include <entityx/entityx.h>

#include "game.h"
#include "events.h"
#include "components.h"

class CharacterSelectionMenu : public engine::core::Scene,
							   public entityx::Receiver<CharacterSelectionMenu> {
 public:
   CharacterSelectionMenu(engine::core::Scene *parent_scene);
   void Update(entityx::TimeDelta dt) override;

   void receive(const StartGame &event);

 private:
   engine::core::Scene *parent_scene_;
};

#endif  // CHARACTER_SELECTION_MENU_H_
#ifndef VICTORY_MENU_H
#define VICTORY_MENU_H 

#include <engine/core/scene.h>
#include <entityx/entityx.h>

#include "game.h"
#include "events.h"
#include "components.h"

class VictoryMenu : public engine::core::Scene,
				  public entityx::Receiver<VictoryMenu> {
	public:
		VictoryMenu(engine::core::Scene *parent_scene);
		void Update(entityx::TimeDelta dt) override;

		void receive(const BackToMainMenu &event);

	private:
		engine::core::Scene *parent_scene_;

};

#endif // VICTORY_MENU_H
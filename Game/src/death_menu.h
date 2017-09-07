#ifndef DEATH_MENU_H_
#define DEATH_MENU_H_ 

#include <engine/core/scene.h>
#include <entityx/entityx.h>

#include "game.h"
#include "events.h"
#include "components.h"

class DeathMenu : public engine::core::Scene,
				  public entityx::Receiver<DeathMenu> {
	public:
		DeathMenu(engine::core::Scene *parent_scene, std::string charac);
		void Update(entityx::TimeDelta dt) override;

		void receive(const BackToMainMenu &event);

	private:
		std::string ch;
		engine::core::Scene *parent_scene_;

};

#endif // DEATH_MENU_H_
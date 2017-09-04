#ifndef TEXT_H_
#define TEXT_H_ 

#include <engine/core/scene.h>
#include <entityx/entityx.h>

#include "game.h"
#include "events.h"
#include "components.h"

using namespace std;

class Text : public engine::core::Scene,
				  public entityx::Receiver<Text> {
	public:
		Text(engine::core::Scene *parent_scene, string text, string name);
		void Update(entityx::TimeDelta dt) override;

		void receive(const BackToGame &event);

	private:
		string text;
		string name;
		engine::core::Scene *parent_scene_;

};

#endif // TEXT_H_
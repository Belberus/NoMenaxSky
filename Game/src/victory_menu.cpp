#include "victory_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite_animation.h>
#include <engine/components/two_d/texture.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_animator.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "entity_factory.h"
#include "systems.h"

using namespace engine::core;
using namespace engine::components::common;
using namespace engine::components::two_d;

VictoryMenu::VictoryMenu(engine::core::Scene *parent_scene)
	: parent_scene_(parent_scene) {

	events.subscribe<BackToMainMenu>(*this);

  	auto camera = entities.create();
  	camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  	camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

  	auto menu_canvas = entities.create();
  	menu_canvas.assign<Transform>(glm::vec3(960.0f / 2.0f, 100.0f, 0.0f));
  	auto menu_canvas_transform = &(*menu_canvas.component<Transform>());

  	entityx::Entity victory = entities.create();

	auto texture = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
	      "assets/victory_menu/pantalla_final.png");
	 
	victory.assign<Sprite>(texture);

	victory.assign<Transform>(glm::vec3(0, 170, 0), menu_canvas_transform,
                          glm::vec3(1.0f));

	// adding systems
	systems.add<engine::systems::two_d::SpriteRenderer>();
	systems.add<VictoryInputSystem>();
	systems.configure();

}

void VictoryMenu::Update(entityx::TimeDelta dt) {
    systems.update<engine::systems::two_d::SpriteRenderer>(dt);
    systems.update<VictoryInputSystem>(dt);
}

void VictoryMenu::receive(const BackToMainMenu &back_to_main) {
	 parent_scene_->events.emit<BackToMainMenu>(back_to_main);
}
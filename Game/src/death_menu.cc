#include "death_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite_animation.h>
#include <engine/components/two_d/sprite.h>
#include <engine/core/audio_manager.h>
#include <engine/components/two_d/texture.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_animator.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "entity_factory.h"
#include "systems.h"
#include "components.h"


using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;

DeathMenu::DeathMenu(engine::core::Scene *parent_scene)
  : parent_scene_(parent_scene) {

  	events.subscribe<BackToMainMenu>(*this);

  	auto camera = entities.create();
  	camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  	camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

  	auto menu_canvas = entities.create();
  	menu_canvas.assign<Transform>(glm::vec3(960.0f / 2.0f, 100.0f, 0.0f));
  	auto menu_canvas_transform = &(*menu_canvas.component<Transform>());

  	entityx::ComponentHandle<Characters> character;

  	auto tex = nullptr;

  	std::vector<engine::utils::Rectangle> death_knight;
  	death_knight.emplace_back(glm::vec2(3, 4), glm::vec2(22, 14));

  	// Lo mismo para wizard

  	for (entityx::Entity e :
  			entities.entities_with_components(character)){
  		if (character->role == Characters::Role::KNIGHT){

  		} else {

  		}
  	}


    systems.add<engine::systems::two_d::SpriteRenderer>();
    systems.add<DeathInputSystem>();
    systems.configure();
}

void DeathMenu::Update(entityx::TimeDelta dt) {
    systems.update<DeathInputSystem>(dt);
  	systems.update<engine::systems::two_d::SpriteRenderer>(dt);
}

void DeathMenu::receive(const BackToMainMenu &back_to_main) {
	parent_scene_->events.emit<BackToMainMenu>(back_to_main);
}
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

#include <fstream>

#include "character_selection_menu.h"

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;

CharacterSelectionMenu::CharacterSelectionMenu(engine::core::Scene *parent_scene)
  : parent_scene_(parent_scene), role(0) {

	events.subscribe<StartGame>(*this);

	auto camera = entities.create();
  	camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  	camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

	auto menu_canvas = entities.create();
  	menu_canvas.assign<Transform>(glm::vec3(960.0f / 2.0f, 100.0f, 0.0f));
  	auto menu_canvas_transform = &(*menu_canvas.component<Transform>());

  	entityx::Entity title = entities.create();
    title.assign<Transform>(glm::vec3(0, 300, 0), menu_canvas_transform,
                          glm::vec3(0.3, 0.3, 1));
    auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
        "assets/personajes_menu/personajes.png");
    title.assign<Sprite>(tex);

    entityx::Entity cursor = entities.create();
    cursor.assign<Transform>(glm::vec3(-155, 0, 0), menu_canvas_transform,
                          glm::vec3(1.1f));
    tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
        "assets/personajes_menu/cursor.png");
    cursor.assign<Sprite>(tex);
    cursor.assign<Cursor>();

	entityx::Entity knight = entities.create();

	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/ppc_front.png");

	knight.assign<Transform>(glm::vec3(-150, 125, 0), 
		menu_canvas_transform, glm::vec3(10.0f));
	knight.assign<Sprite>(tex);


	entityx::Entity wizard = entities.create();

	tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/ppc_front.png");

	wizard.assign<Transform>(glm::vec3(150, 125, 0), 
		menu_canvas_transform, glm::vec3(10.0f));
	wizard.assign<Sprite>(tex);

	systems.add<engine::systems::two_d::SpriteAnimator>();
  	systems.add<engine::systems::two_d::SpriteRenderer>();
  	//systems.add<SelectionInputSystem>();
  	systems.configure();
}

void CharacterSelectionMenu::Update(entityx::TimeDelta dt){

	entityx::ComponentHandle<Characters> character;

	entityx::ComponentHandle<Cursor> cursor;
	entityx::ComponentHandle<Sprite> sprite;
	entityx::ComponentHandle<Transform> transform;

	for (entityx::Entity e :
			entities.entities_with_components(character)){
		// Cursor en mago cambiar a caballero
		if (character->role == Characters::Role::KNIGHT && role == 1){
			for (entityx::Entity e1 :
					entities.entities_with_components(cursor, sprite, transform)){
				transform->SetLocalPosition(glm::vec3(-155, 0, 0));
			}
		}

		if (character->role == Characters::Role::WIZARD && role == 0){
			for (entityx::Entity e1 :
					entities.entities_with_components(cursor, sprite, transform)){
				transform->SetLocalPosition(glm::vec3(145, 0, 0));
			}
		}

	}

	//systems.update<OptionsInputSystem>(dt);
  	systems.update<engine::systems::two_d::SpriteAnimator>(dt);
  	systems.update<engine::systems::two_d::SpriteRenderer>(dt);
}

void CharacterSelectionMenu::receive(const StartGame &event){
	parent_scene_->events.emit<StartGame>(event);
}
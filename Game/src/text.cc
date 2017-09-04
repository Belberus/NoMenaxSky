#include "text.h"

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
#include "text_factory.h"

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;
using namespace std;

Text::Text(engine::core::Scene *parent_scene, string text)
  : parent_scene_(parent_scene), text(text){
  	events.subscribe<BackToGame>(*this);

  	auto camera = entities.create();
  	camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  	camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

  	auto text_canvas = entities.create();
  	text_canvas.assign<Transform>(glm::vec3(960.0f / 2.0f, 400.0f, 0.0f));
  	auto text_canvas_transform = &(*text_canvas.component<Transform>());

  	entityx::ComponentHandle<Characters> character;
    entityx::ComponentHandle<Cursor> cursor;

    string route = textToImage(text, glm::vec2(400,100), "bienvenido");

    auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
          route);
    entityx::Entity text_entity = entities.create();
    text_entity.assign<Transform>(glm::vec3(0,-200,0), text_canvas_transform,
    	glm::vec3(2.0f));
    text_entity.assign<Sprite>(tex);

    systems.add<engine::systems::two_d::SpriteRenderer>();
    systems.add<TextInputSystem>();
    systems.configure();
  }

  void Text::Update(entityx::TimeDelta dt) {
  	systems.update<TextInputSystem>(dt);
  	systems.update<engine::systems::two_d::SpriteRenderer>(dt);
  }

  void Text::receive(const BackToGame &back_to_game) {
	  parent_scene_->events.emit<BackToGame>(back_to_game);
}
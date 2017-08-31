#include "options_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite_animation.h>
#include <engine/components/two_d/sprite.h>
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


OptionsMenu::OptionsMenu(engine::core::Scene *parent_scene) 
	: parent_scene_(parent_scene) {

  auto menu = entities.create();
  menu.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 0), nullptr,
                         glm::vec3(960.0f / 500.0f, 540.0f / 477.0f, 1.0f));

  auto camera = entities.create();
  camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);

  std::vector<engine::utils::Rectangle> rect;
  rect.emplace_back(glm::vec2(0, 0), glm::vec2(500, 477));
  rect.emplace_back(glm::vec2(500, 0), glm::vec2(500, 477));
  rect.emplace_back(glm::vec2(1000, 0), glm::vec2(500, 477));
  rect.emplace_back(glm::vec2(1500, 0), glm::vec2(500, 477));
  auto texture = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/background_menu.png");
  SpriteAnimation::AnimationClip background_anim("background_menu", texture,
                                                 rect, 100.0f);
  SpriteAnimation anim({background_anim});
  menu.assign<SpriteAnimation>(std::move(anim));
  menu.assign<Sprite>(texture);

  auto logo = entities.create();
  logo.assign<Transform>(glm::vec3(910, 50, 0), nullptr,
                         glm::vec3(0.1, 0.1, 1));
  auto logo_tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/patan_games.png");
  logo.assign<Sprite>(logo_tex);

  auto menu_canvas = entities.create();
  menu_canvas.assign<Transform>(glm::vec3(960.0f / 3.0f, 200.0f, 0.0f));
  auto menu_canvas_transform = &(*menu_canvas.component<Transform>());

  entityx::Entity exit_option = entities.create();
  exit_option.assign<Transform>(glm::vec3(0, -70, 0), menu_canvas_transform);
  auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/salir.png");
  exit_option.assign<Sprite>(tex);

  // arrow
  entityx::Entity menuArrow = entities.create();
  menuArrow.assign<Transform>(glm::vec3(-90, 140, 0), menu_canvas_transform,
                              glm::vec3(2, 2, 1));
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/ppc_front.png");
  menuArrow.assign<Sprite>(tex);

  menuArrow.assign<ArrowOptions>(ArrowOptions::Option::MODE);

  GameOptions opciones_iniciales = {
  		GameOptions::Modo::TWO_D, 
  		GameOptions::Musica::MUSIC_ON,
  		GameOptions::Efectos::FX_ON 
  	};

  menuArrow.assign<GameOptions>(opciones_iniciales);

  systems.add<engine::systems::two_d::SpriteAnimator>();
  systems.add<engine::systems::two_d::SpriteRenderer>();
  systems.add<OptionsInputSystem>();
  systems.configure();

}

void OptionsMenu::Update(entityx::TimeDelta dt) {
  systems.update<OptionsInputSystem>(dt);
  systems.update<engine::systems::two_d::SpriteAnimator>(dt);
  systems.update<engine::systems::two_d::SpriteRenderer>(dt);
}

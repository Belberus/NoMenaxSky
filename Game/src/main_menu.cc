#include "main_menu.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite.h>
#include <engine/components/two_d/texture.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "systems.h"
#include "entity_factory.h"

using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::core;

MainMenu::MainMenu(engine::core::Scene* parent_scene)
    : parent_scene_(parent_scene) {
  events.subscribe<StartGame>(*this);

  // adding entities
  auto camera = entities.create();
  camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1));
  camera.assign<Camera>(960.0f, 540.0f);

  auto menu_canvas = entities.create();
  menu_canvas.assign<Transform>(glm::vec3(960.0f / 2.0f, 100.0f, 0.0f));
  auto menu_canvas_transform = &(*menu_canvas.component<Transform>());

  // exit option
  entityx::Entity exit_option = entities.create();
  exit_option.assign<Transform>(glm::vec3(0, 0, 0), menu_canvas_transform);
  auto tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/salir.png");
  exit_option.assign<Sprite>(tex);

  // options option
  entityx::Entity options_option = entities.create();
  options_option.assign<Transform>(glm::vec3(0, 70, 0), menu_canvas_transform);
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/opciones.png");
  options_option.assign<Sprite>(tex);

  // play option
  entityx::Entity play_option = entities.create();
  play_option.assign<Transform>(glm::vec3(0, 140, 0), menu_canvas_transform);
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/jugar.png");
  play_option.assign<Sprite>(tex);

  // title
  entityx::Entity title = entities.create();
  title.assign<Transform>(glm::vec3(0, 300, 0), menu_canvas_transform,
                          glm::vec3(0.3, 0.3, 1));
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/titulo.png");
  title.assign<Sprite>(tex);

  // arrow
  entityx::Entity menuArrow = entities.create();
  menuArrow.assign<Transform>(glm::vec3(-90, 140, 0), menu_canvas_transform,
                              glm::vec3(2, 2, 1));
  tex = Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
      "assets/menu/ppc_front.png");
  menuArrow.assign<Sprite>(tex);
  menuArrow.assign<ArrowMenu>(ArrowMenu::Option::JUGAR);

  // adding systems
  systems.add<engine::systems::two_d::SpriteRenderer>();
  systems.add<MenuInputSystem>();
  systems.configure();
}

void MainMenu::Update(entityx::TimeDelta dt) {
  systems.update<MenuInputSystem>(dt);
  systems.update<engine::systems::two_d::SpriteRenderer>(dt);
}

void MainMenu::receive(const StartGame& start_game) {
  parent_scene_->events.emit<StartGame>(start_game);
}

void MainMenu::AddEntities() {}

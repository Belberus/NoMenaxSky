#include "game_ui.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_renderer.h>

using namespace engine::core;
using namespace engine::systems::two_d;
using namespace engine::components::two_d;
using namespace engine::components::common;
using namespace engine::components::two_d;
using namespace engine::utils;

GameUi::GameUi(Game* parent_scene) {
  // adding entities
  auto gui_texture_atlas =
      Engine::GetInstance().Get<ResourceManager>().Load<Texture>(
          "assets/spritesheets/gui.png");

  auto camera = entities.create();
  camera.assign<Transform>(glm::vec3(960.0f / 2.0f, 540.0f / 2.0f, 1.0f));
  camera.assign<Camera>(960.0f, 540.0f);
  auto health_bar = entities.create();
  health_bar.assign<Transform>(glm::vec3(100.0f, 520.0f, 0.0f), nullptr,
                               glm::vec3(17.0f, 17.0f, 1.0f));
  health_bar.assign<Sprite>(
      gui_texture_atlas,
      Rectangle(glm::vec2(3.0f, 20.0f), glm::vec2(10.0f, 1.0f)));

  // auto stamina_bar = entities.create();
  // stamina_bar.assign<Transform>(glm::vec3(100.0f, 490.0f, 0.0f), nullptr,
  //                              glm::vec3(17.0f, 17.0f, 1.0f));
  // stamina_bar.assign<Sprite>(
  //    gui_texture_atlas,
  //    Rectangle(glm::vec2(3.0f, 17.0f), glm::vec2(10.0f, 1.0f)));

  // adding systems
  systems.add<SpriteRenderer>();
  systems.configure();
}

void GameUi::Update(entityx::TimeDelta dt) {
  systems.update<SpriteRenderer>(dt);
}

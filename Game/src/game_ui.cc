#include "game_ui.h"

#include <engine/components/common/camera.h>
#include <engine/components/common/transform.h>
#include <engine/components/two_d/sprite.h>
#include <engine/core/engine.h>
#include <engine/core/resource_manager.h>
#include <engine/systems/two_d/sprite_renderer.h>

#include "components.h"

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
  camera.assign<Camera>(960.0f, 540.0f, 0.1f, 1000.0f);
  auto health_bar = entities.create();
  health_bar.assign<WhatOption>(1);
  health_bar.assign<Transform>(glm::vec3(100.0f, 520.0f, 0.0f), nullptr,
                               glm::vec3(17.0f, 17.0f, 1.0f));
  health_bar.assign<Sprite>(
      gui_texture_atlas,
      Rectangle(glm::vec2(3.0f, 20.0f), glm::vec2(10.0f, 1.0f)));

  parent_scene->events.subscribe<Health>(*this);

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

int i = 0;
void GameUi::receive(const Health& health) {
  std::cout << (health.hp / health.init_hp) << std::endl;
  entities.each<Transform, WhatOption>(
    [&](entityx::Entity health_bar, Transform &transform,
      WhatOption &wo){
      auto scale = transform.GetLocalScale();
      auto position = transform.GetLocalPosition();
      if(init_x == 0){
        init_x = scale.x;
      }
      if(i == 0){
        i++;
        init_pos = position.x;
      }
      float result = init_x * (health.hp / health.init_hp);
      if(result > 0){
        scale.x = result;
      }
      else scale.x = 0;
      transform.SetLocalScale(scale);

      std::cout << "init pos: " << init_pos << std::endl;

      float newpos = init_pos - (170 - (170*(health.hp / health.init_hp))/2.0f);
      std::cout << "new pos: " << newpos << std::endl;
      transform.SetLocalPosition(glm::vec3(newpos,position.y,position.z));
    });
}

  /*Engine::GetInstance().Get<EntityManager>().each<health_bar, Player>(
    [&](entityx::Entity health_bar, entityx::Entity Player){
      std::cout << "pss" << std::endl;
    });*/
